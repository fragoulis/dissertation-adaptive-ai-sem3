#include "kNN.h"
#include "CAction.h"
#include "logger.h"
#include "random.h"
#include "Map.h"
#include "Grid.h"
#include "CUnitManager.h"
#include "config.h"
#include "Renderer.h"
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
m_step(STATE_CHOOSE_ACTION),
K(5),
m_updateThreshold(0.7)
{
#ifndef _RANDOM_LEARNING_ON_
    Renderer::Get().AddObject(this);
#endif
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
    if( m_casebase.size() == 0 )
    {
        _RunRandomly(newcase, unit);
        return;
    }

    // Retrieve latest case path which we will need to update
    CCasePath& casepath = _Initialize();

    // ________________________________________________________________________
    if( STATE_CHOOSE_ACTION == m_step )
    {
        _log_2n("Unit is at: ", unit.GetGridPosition().X, ", ", unit.GetGridPosition().Y);

        _ChooseAction(newcase);

        m_step = STATE_PAUSED;
    }

    // ________________________________________________________________________
    if( STATE_GET_FITNESS == m_step )
    {
        CCase *oc = _ActOnAction(unit);
        if( oc )
        {
            // If case comparison does not find anything similar, go ahead
            // and add the new case.
            newcase.SetAction(m_action);
            newcase.SetFitness(m_fitness);

            _log_2n("Adding new case...[action=", action_name(m_action), " fitness=", m_fitness );

            // Push case to casebase.
            m_casebase.push_back(newcase);

            _Finalize(casepath, & (m_casebase.back()), unit);
        }
        else
        {
            _Finalize(casepath, oc, unit);
        }

        m_step = STATE_CHOOSE_ACTION;
    }

}

// ----------------------------------------------------------------------------
void kNN::_ChooseAction( CCase &newcase )
{
    E_ACTION chosen_action  = DOWN;
    const int casebase_size = (int) m_casebase.size();
    const int actual_k      = ( K > casebase_size ) ? casebase_size : K;
    
    m_k_cases.clear();
    m_k_cases.resize(actual_k);

    // Iterate through the whole casebase and save the K-cases closest to the 
    // queried case.
    CaseBase::iterator it = m_casebase.begin();
    for(; it != m_casebase.end(); ++it )
    {
        bool placed = false;
        CCase &oldcase = (*it);
        oldcase.SetText(0);

        const int d = oldcase.ComputeDistance( newcase );
        assert(oldcase.GetAction()>=0);
        for( int i=0; i<actual_k && !placed; i++ )
        {
            if( d < m_k_cases[i].distance )
            {
                // if it is,
                // move all distances one position to the right
                for( int n=actual_k-1; n>i; n-- )
                {
                    m_k_cases[n] = m_k_cases[n-1];
                }
            
                // put current distance in
                m_k_cases[i].distance = d;
                m_k_cases[i].oCase = &oldcase;

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
    for( int i=0; i<(int)m_k_cases.size(); i++ )
    {
        double fitness = m_k_cases[i].oCase->GetFitness();
        int act = (int) m_k_cases[i].oCase->GetAction();
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

    m_action = chosen_action;
}

// ----------------------------------------------------------------------------
CCase* kNN::_ActOnAction(CUnit &unit )
{
    // Run the action and retrieve a fitness
    m_fitness = unit.ActOn(m_action);

    _CheckFitness();

    // Iterate through all cases and compare the similarity with the
    // queried case. If two cases are the same (distance equals zero)
    // update, do not add the new case, but instead average the fitness
    // of the two.
    _k_cases::iterator cit = m_k_cases.begin();
    for(; cit != m_k_cases.end(); ++cit )
    {
        _case &oldcase = (*cit);
        CCase *pCase = oldcase.oCase;
        
        if( 0 == oldcase.distance )
        {   
            if( m_action == pCase->GetAction() )
            {
                if( !irr::core::equals( (float)pCase->GetFitness(), (float)m_fitness ) )
                {
                    _log_2n("Updating case...[Current: ", pCase->GetFitness(), " New: ", m_fitness);
        
                    // If the cases have same state and action, instead of adding
                    // a new case, update the fitness of the existing case.
                    pCase->SetFitness( (pCase->GetFitness() + m_fitness) * 0.5 );

                    _log_n("Average case: ", pCase->GetFitness());
                }

                // Finally return. There shouldn't be anynore checks because,
                // since we found a similar case already we SHOULD NOT find 
                // another one.
                return pCase;
            }
        }

    } // for 

    return 0;
}

// ----------------------------------------------------------------------------
void kNN::_RunRandomly( CCase &newcase, CUnit &unit )
{
    // Retrieve latest case path which we will need to update
    CCasePath& casepath = _Initialize();

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
                
                _Finalize(casepath, &oldcase, unit);

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

    _Finalize(casepath, & (m_casebase.back()), unit);
}

// ----------------------------------------------------------------------------
void kNN::_CheckFitness()
{
    // If fitness is zero and the agent is dead, stop the algo.
    if( m_fitness < 0.0001 )
    {
        _Stop(); // death
    }
    else if( m_fitness > 1.0 ) // completion
    {
        _Stop();
        m_fitness = 1.0;
    }
}

// ----------------------------------------------------------------------------
CCasePath& kNN::_Initialize()
{
    // If algorithm is starting fresh create a new case path
    if( _IsAlgoStarting() )
    {
        m_casepaths.push_back(CCasePath());
        _Run();
    }

    // Retrieve latest case path which we will need to update
    return m_casepaths.back();
}

// ----------------------------------------------------------------------------
void kNN::_Finalize( CCasePath &cp, CCase *c, CUnit &unit )
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
        _Start();
        unit.Reset();
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

    if(!case_out.is_open()) return;

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
}

// ----------------------------------------------------------------------------
void kNN::SaveResults() const
{
    std::string file = "data/results/";
    file += m_fileResults;
    file += ".dat";

    std::ofstream path_out( file.c_str() );
    assert(path_out.is_open());

    if(!path_out.is_open()) return;

    double total_fitness = 0.0;
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

            total_fitness += (*it).GetFitness();
        }
    }

    path_out << "Average fitness = ";
    path_out << (total_fitness / size);
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

    if(!case_in.is_open()) return;

    int cs = 0;
    while(!case_in.eof())
    {
        //_log_n("Adding case ... ", cs++);
        CCase newcase;
        newcase.Load(case_in);
        AddCase(newcase);
    }
}

// ----------------------------------------------------------------------------
void kNN::Render() const
{
    Grid& grid = Grid::Get();
    const CUnit &unit = MgrUnit::Get().GetActor();
    const int MAX_CASES_PER_LINE = 5;
    const int MAX_CASE_WIDTH = ( config::wndSize.Width - grid.width - 2*grid.tilesize.Width ) / MAX_CASES_PER_LINE;

    CState::s_scaled.scale = (float)MAX_CASE_WIDTH / grid.width;
    CState::s_scaled.fov = unit.GetFOV();
    CState::s_scaled.tileWidth  = int((float)grid.tilesize.Width * CState::s_scaled.scale);
    //CState::s_scaled.tileHeight = int((float)grid.tilesize.Height * CState::s_scaled.scale);
    CState::s_scaled.tileHeight = CState::s_scaled.tileWidth;
    CState::s_scaled.gridWidth  = CState::s_scaled.tileWidth * grid.tilesX;
    CState::s_scaled.gridHeight = CState::s_scaled.tileHeight * unit.GetFOV();

    const int startX = grid.width + grid.tilesize.Width;
    const int startY = 300;

    int count = 0;
    _k_cases::const_iterator it = m_k_cases.begin();
    for(; it != m_k_cases.end(); ++it, ++count )
    {
        const CCase &c = * (*it).oCase;
        const CState &state = c.GetState();

        int cx = count % MAX_CASES_PER_LINE;
        int cy = ( count - cx ) / MAX_CASES_PER_LINE;
        int x = startX + cx * ( (grid.tilesize.Width>>1) + CState::s_scaled.gridWidth );
        int y = startY + cy * ( (grid.tilesize.Height<<3) + CState::s_scaled.gridHeight );
        state.SetPosition( position2di(x, y) );

        c.Render();
    }
}