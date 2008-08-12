#include "CCasePath.h"
#include "Grid.h"
#include "CCase.h"
#include "CUnitManager.h"

// ----------------------------------------------------------------------------
CCasePath::CCasePath():
m_fitness(0.0),
m_closed(false)
{
}

// ----------------------------------------------------------------------------
bool CCasePath::CheckSize() const 
{
    return ( (int)m_caselist.size() < Grid::Get().tilesY );
}

// ----------------------------------------------------------------------------
void CCasePath::_UpdateFitness()
{
    m_fitness = m_caselist.size() / double( Grid::Get().tilesY - 1 );
}

// ----------------------------------------------------------------------------
void CCasePath::Close()
{
    _UpdateFitness();
    m_closed = true;
}

// ----------------------------------------------------------------------------
void CCasePath::Update()
{
    assert(m_closed!=true);

    // We need to know how far the agent can see to avoid updating arbitrary
    // number of cases.
    const int fov = MgrUnit::Get().GetActor().GetFOV();

    int current = 0;

    Caselist::iterator it = m_caselist.begin();

    Caselist::iterator prev = it;

    // Move one to avoid adjusting the fitness for the last case which action
    // led to death.
    ++it;

    for(; 
        it != m_caselist.end() && current != fov; 
        ++it, ++prev, ++current )
    {
        CCase *c = *it;

        c->SetFitness( 
            ( c->GetFitness() + (*prev)->GetFitness() ) * 0.5
            );

        assert((c->GetFitness()>=0.0)&&(c->GetFitness()<=1.0));
    }
}

// ----------------------------------------------------------------------------
void CCasePath::Save( std::ostream &out ) const
{
    out << "Number of steps=" << m_caselist.size();
    //out << ", ";
    //out << "Health=" << MgrUnit::Get().GetActor().GetHealth();
    out << ", ";
    out << "Fitness=" << m_fitness;
    out << std::endl;
}