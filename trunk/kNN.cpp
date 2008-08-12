#include "kNN.h"
#include "CAction.h"
#include "logger.h"
#include "random.h"
#include "Map.h"
#include <string>

//#define _RANDOM_LEARNING_ON_

// ----------------------------------------------------------------------------
#ifdef _DEBUG
const char* action_name(int i)
{
    switch(i)
    {
    case DOWN: return "Down";
    case LEFT: return "Left";
    case RIGHT: return "Right";
    }

    assert(0);
    return 0;
}
#endif

// ----------------------------------------------------------------------------
kNN::kNN():
m_state(ALGO_STARTING),
K(5),
m_updateThreshold(0.7)
{
}

// ----------------------------------------------------------------------------
kNN::~kNN()
{
#ifdef _RANDOM_LEARNING_ON_
    Save();
#else
    SaveResults();
#endif
}

// ----------------------------------------------------------------------------
void kNN::Initialize( IrrXMLReader *xml )
{
    K = xml->getAttributeValueAsInt("k");
    m_updateThreshold = (double)xml->getAttributeValueAsFloat("threshold");
    int casepaths_reserve = xml->getAttributeValueAsInt("casepaths");
    int casebase_reserve = 750;

    // read consiquent children tags
    while(xml->read())
    {
        switch(xml->getNodeType())
        {
        case EXN_ELEMENT:
            {
                if( !strcmp( "casebase", xml->getNodeName() ) )
                {
#ifndef _RANDOM_LEARNING_ON_
                    m_fileCasebase = xml->getAttributeValue("in");
#endif
                    casebase_reserve = xml->getAttributeValueAsInt("size");
                }
            }
            break;
        }
    } // while

    std::string mapfile = Map::Get().GetName();
    
    // The next two lines transform the filename from
    // data/maps/mapXX.xml to
    // mapXX
    mapfile.replace(0, mapfile.size() - 9, "");
    mapfile.replace(5, mapfile.size(), "");

#ifdef _RANDOM_LEARNING_ON_
    // Construct the casebase filename according to the map that was 
    // initially used to train the casbase.
    m_fileCasebase = mapfile;
#endif

    // Construct the results filename depending on the the casebase file,
    // the current map and the K parametre.

    char buf[3];
    ::_itoa_s(K, buf, 10);

    m_fileResults  = m_fileCasebase;
    m_fileResults += "_";
    m_fileResults += mapfile;
    m_fileResults += "_";
    m_fileResults += buf;

    m_casebase.reserve(casebase_reserve);
    m_casepaths.reserve(casepaths_reserve);

#ifndef _RANDOM_LEARNING_ON_
    Load();
#endif

}

// ----------------------------------------------------------------------------
void kNN::RunAlgo( CCase &newcase, CUnit &unit )
{
#ifdef _RANDOM_LEARNING_ON_
    _RunRandomly(newcase, unit);
#else
    _RunNormal(newcase, unit);
#endif
}

// ----------------------------------------------------------------------------
void kNN::_RunNormal( CCase &newcase, CUnit &unit )
{
    _log_2n("Unit is at: ", unit.GetGridPosition().X, ", ", unit.GetGridPosition().Y);

    E_ACTION chosen_action  = DOWN;
    const int casebase_size = (int) m_casebase.size();
    const int actual_k      = ( K > casebase_size ) ? casebase_size : K;
    std::vector<_case> cases(actual_k);

    // Retrieve latest case path which we will need to update
    CCasePath& casepath = _Initialize(unit);

    // Iterate through the whole casebase and save the K-cases closest to the 
    // queried case.
    CaseBase::iterator it = m_casebase.begin();
    for(; it != m_casebase.end(); ++it )
    {
        bool placed = false;
        CCase &oldcase = (*it);
        const int d = oldcase.ComputeDistance( newcase );
        assert(oldcase.GetAction()>=0);
        for( int i=0; i<actual_k && !placed; i++ )
        {
            if( d < cases[i].distance )
            {
                // if it is,
                // move all distances one position to the right
                for( int n=actual_k-1; n>i; n-- )
                {
                    cases[n] = cases[n-1];
                }
            
                // put current distance in
                cases[i].distance = d;
                cases[i].oCase = &oldcase;

                placed = true;
            } // if 

        } // for
    } // for 

    // ________________________________________________________________________
    
    // Declare a vector of the available actions. This will hold the number of 
    // actions types followed and the average fitness for each action type.
    std::vector<_action> action_poll(NUM_OF_ACTIONS);
    for( int i=0; i<NUM_OF_ACTIONS; i++ )
        action_poll[i].action = (E_ACTION)i;

    // Go through the K-cases and extract the action population and
    // corresponding fitness.
    for( int i=0; i<(int)cases.size(); i++ )
    {
        double fitness = cases[i].oCase->GetFitness();
        int act = (int) cases[i].oCase->GetAction();
        assert(act>=0);

        action_poll[act].votes++;
        action_poll[act].avg_fitness += fitness;
    }

    // For every action update the average fitness.
    for( int i=0; i<NUM_OF_ACTIONS; i++ )
    {   
        _action& ap = action_poll[i];

        if( ap.votes > 0 )
            ap.avg_fitness /= ap.votes;
        else
            ap.avg_fitness = 1.0;
    }

#ifdef _DEBUG
    // Print the K-cases
    for( int i=0; i<NUM_OF_ACTIONS; i++ )
        std::cout << action_name(i) << "(" << action_poll[i].votes << ", " << action_poll[i].avg_fitness << ") ";
#endif

    // Sort action poll according to average fitness from higher to lower.
    _SortActions(action_poll);
    chosen_action = action_poll[0].action;
    
    // See if there are more than one with best fitness.
    for( int i=1; i<NUM_OF_ACTIONS; i++ )
    {
        if( action_poll[0].avg_fitness == action_poll[i].avg_fitness )
        {
            if( action_poll[0].votes < action_poll[i].votes )
            {
                chosen_action = action_poll[i].action;
            }
        }
    }

    assert(chosen_action>=0);
#ifdef _DEBUG
    std::cout << action_name(chosen_action) << std::endl;
#endif

    // Run the action and retrieve a fitness
    m_fitness = unit.ActOn(chosen_action);

    _log_2n("Unit moved to: ", unit.GetGridPosition().X, ", ", unit.GetGridPosition().Y);

    _CheckFitness();

    // Iterate through all cases and compare the similarity with the
    // queried case. If two cases are the same (distance equals zero)
    // update, do not add the new case, but instead average the fitness
    // of the two.
    std::vector<_case>::iterator cit = cases.begin();
    for(; cit != cases.end(); ++cit )
    {
        _case &oldcase = (*cit);
        CCase *pCase = oldcase.oCase;
        
        if( 0 == oldcase.distance )
        {   
            if( chosen_action == pCase->GetAction() )
            {
                if( !irr::core::equals( (float)pCase->GetFitness(), (float)m_fitness ) )
                {
                    _log_2n("Updating case...[Current: ", pCase->GetFitness(), " New: ", m_fitness);
        
                    // If the cases have same state and action, instead of adding
                    // a new case, update the fitness of the existing case.
                    pCase->SetFitness( (pCase->GetFitness() + m_fitness) * 0.5 );

                    _log_n("Average case: ", pCase->GetFitness());
                }

                _Finalize(casepath, pCase);

                // Finally return. There shouldn't be anynore checks because,
                // since we found a similar case already we SHOULD NOT find 
                // another one.
                return;
            }
        }

    } // for 

    // If case comparison does not find anything similar, go ahead
    // and add the new case.
    newcase.SetAction((E_ACTION)chosen_action);
    newcase.SetFitness(m_fitness);

    _log_2n("Adding new case...[action=", action_name(chosen_action), " fitness=", m_fitness );

    // Push case to casebase.
    m_casebase.push_back(newcase);

    _Finalize(casepath, & (m_casebase.back()));
}

// ----------------------------------------------------------------------------
void kNN::_RunRandomly( CCase &newcase, CUnit &unit )
{
    _log_2n("Unit is at: ", unit.GetGridPosition().X, ", ", unit.GetGridPosition().Y);

    // Retrieve latest case path which we will need to update
    CCasePath& casepath = _Initialize(unit);

    // Choose a random action
    E_ACTION chosen_action = (E_ACTION)(rand()%NUM_OF_ACTIONS);

    // Run the action and retrieve a fitness
    m_fitness = unit.ActOn(chosen_action);

    _CheckFitness();

    // Iterate through all cases and compare the similarity with the
    // queried case. If two cases are the same (distance equals zero)
    // update, do not add the new case, but instead average the fitness
    // of the two.
    CaseBase::iterator it = m_casebase.begin();
    for(; it != m_casebase.end(); ++it )
    {
        CCase &oldcase = (*it);
        const int d = oldcase.ComputeDistance( newcase );
        
        if( d == 0 )
        {   
            if( chosen_action == oldcase.GetAction() )
            {
                if( !irr::core::equals( (float)oldcase.GetFitness(), (float)m_fitness ) )
                {
                    _log_2n("Updating case...[Current: ", oldcase.GetFitness(), " New: ", m_fitness);

                    // If the cases have same state and action, instead of adding
                    // a new case, update the fitness of the existing case.
                    oldcase.SetFitness( 
                        (oldcase.GetFitness() + m_fitness) * 0.5
                        );

                    assert((oldcase.GetFitness()>=0.0)&&(oldcase.GetFitness()<=1.0));

                    _log_n("Average case: ", oldcase.GetFitness());
                }
                
                _Finalize(casepath, &oldcase);

                // Finally return. There shouldn't be anynore checks because,
                // since we found a similar case already we SHOULD NOT find 
                // another one.
                return;
            }
        }

    } // for 

    // If case comparison does not find anything similar, go ahead
    // and add the new case.
    newcase.SetAction((E_ACTION)chosen_action);
    newcase.SetFitness(m_fitness);

    _log_2n("Adding new case...[action=", action_name(chosen_action), " fitness=", m_fitness );

    // Push case to casebase.
    m_casebase.push_back(newcase);

    _Finalize(casepath, & (m_casebase.back()));
}

// ----------------------------------------------------------------------------
void kNN::_CheckFitness()
{
    // If fitness is zero and the agent is dead, stop the algo.
    if( m_fitness < 0.0001 )
    {
        m_state = ALGO_STOPPING; // death
    }
    else if( m_fitness > 1.0 ) // completion
    {
        m_state = ALGO_STOPPING;
        m_fitness = 1.0;
    }
}

// ----------------------------------------------------------------------------
CCasePath& kNN::_Initialize( CUnit &unit )
{
    // If algorithm is starting fresh create a new case path
    if( _IsAlgoStarting() )
    {
        m_casepaths.push_back(CCasePath());
        m_state = ALGO_RUNNING;
        unit.Reset();
    }

    // Retrieve latest case path which we will need to update
    return m_casepaths.back();
}

// ----------------------------------------------------------------------------
void kNN::_Finalize( CCasePath &cp, CCase *c )
{
    // Add this case to history.
    cp.AddCase( c );
    assert(cp.CheckSize());

    // If fitness is below this threshold, update the path to this point
    // average with every other case.
    if( m_fitness < m_updateThreshold ) cp.Update();

    // If algorithm is stopping fininalize the case path.
    if( _IsAlgoStopping() )
    {
        cp.Close();
        m_state = ALGO_STARTING;
    }
}

// ----------------------------------------------------------------------------
void kNN::Write( stringw &out ) const
{
    out += "\nCases: "; 
    out += (int)m_casebase.size();
    out += "\nTries: "; 
    out += (int)m_casepaths.size();
    out += "\n";
}

// ----------------------------------------------------------------------------
void kNN::_SortActions( std::vector<_action> &actions ) const
{
    bool no_swaps;
    do 
    {
        no_swaps = true;
        for( int i=0; i<(int)actions.size()-1; i++ )
        {
            if( actions[i].avg_fitness < actions[i+1].avg_fitness )
            {
                _action temp = actions[i];
                actions[i] = actions[i+1];
                actions[i+1] = temp;
                no_swaps = false;
            }
        }
    }
    while(!no_swaps);
}

// ----------------------------------------------------------------------------
void kNN::Save() const
{
    std::string file = "data/casebase/";
    file += m_fileCasebase;
    file += ".dat";

    std::ofstream case_out( file.c_str() );
    assert(case_out.is_open());

    const int size = (int)m_casebase.size();
    CaseBase::const_iterator it = m_casebase.begin();
    for( 
        int cur = 0;
        it != m_casebase.end(); 
        ++it, ++cur )
    {
        (*it).Save(case_out);

        if( cur < size-1 ) 
            case_out << std::endl;
    }

    case_out;
}

// ----------------------------------------------------------------------------
void kNN::SaveResults() const
{
    std::string file = "data/results/";
    file += m_fileResults;
    file += ".dat";

    std::ofstream path_out( file.c_str() );
    assert(path_out.is_open());

    const int size = (int)m_casepaths.size();
    CasePaths::const_iterator it = m_casepaths.begin();
    for(
        int cur = 1; 
        it != m_casepaths.end(); 
        ++it, ++cur )
    {
        if( cur < size-1 )
        {
            path_out << cur << ": ";

            (*it).Save(path_out);
        }
    }
}

// ----------------------------------------------------------------------------
void kNN::Load()
{
    m_casebase.clear();

    std::string file = "data/casebase/";
    file += m_fileCasebase;
    file += ".dat";

    std::ifstream case_in( file.c_str() );
    assert(case_in.is_open());

    int cs = 0;
    while(!case_in.eof())
    {
        //_log_n("Adding case ... ", cs++);
        CCase newcase;
        newcase.Load(case_in);
        AddCase(newcase);
    }
}
