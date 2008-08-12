#include "CCase.h"
#include "CColorManager.h"

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

// ----------------------------------------------------------------------------
void CCase::Render() const
{
    m_state.Render();

    // get the last feature
    int X = m_state.GetFeature(-1);
    if(m_action.GetDirection() == LEFT) X--;
    else if(m_action.GetDirection() == RIGHT) X++;


    DrawRectangle(
        * (MgrColor::Get().GetData("red")), 
        position2di(
            m_state.GetPosition().X + X * CState::s_scaled.tileWidth, 
            m_state.GetPosition().Y
            ),
        dimension2di(
            CState::s_scaled.tileWidth,
            CState::s_scaled.tileHeight
            )
        );

    stringw str;
    str += L"fitness=";
    str += m_fitness;
    m_text->setText(str.c_str());

    int x = m_state.GetPosition().X;
    int y = m_state.GetPosition().Y + CState::s_scaled.gridHeight;
    m_text->setRelativePosition(
        rect<s32>(x, y, x+CState::s_scaled.gridWidth, y+10)
        );
}