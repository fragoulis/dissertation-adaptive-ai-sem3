#include "CCase.h"

#define _OUT(label, value) out += label; out += value; out += "\n";

// ----------------------------------------------------------------------------
CCase::CCase():
m_fitness(1.0)
{
}

// ----------------------------------------------------------------------------
void CCase::Initialize( const CUnit &unit )
{
    m_state.Initialize(unit);
}

// ----------------------------------------------------------------------------
int CCase::ComputeDistance( const CCase &other ) const
{
    return m_state.ComputeDistance(other.GetState());
}

// ----------------------------------------------------------------------------
void CCase::Save( std::ostream &out ) const
{
    out << m_fitness << " ";
    out << m_action.GetDirection() << " ";
    m_state.Save(out);
}

// ----------------------------------------------------------------------------
void CCase::Load( std::istream &in )
{
    int action_direction;

    in >> m_fitness;
    in >> action_direction;
    m_state.Load(in);

    m_action.SetDirection((E_ACTION)action_direction);
}