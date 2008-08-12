#include "CUnit.h"
#include "CTeam.h"
#include "Grid.h"
#include "logger.h"
#include "CObjectTypeManager.h"
#include "CColorManager.h"
#include "Random.h"

#define _OUT(label, value) out += label; out += value; out += "\n";

#define _PUNISH_DIAGONAL_

// extra action
const int NEUTRAL = -1;

// ----------------------------------------------------------------------------
_debug CUnit::debug;

// ----------------------------------------------------------------------------
CUnit::CUnit():
m_action(NEUTRAL)
{
    debug.drawFOV = false;
    debug.drawDir = false;
    debug.drawId = false;
}

// ----------------------------------------------------------------------------
double CUnit::ActOn( E_ACTION action )
{
    int health = m_iHealth;

    Grid& grid = Grid::Get();
    MgrObjectType& mgr = MgrObjectType::Get();

    IObject *occ = 0;
    switch(action)
    {
    case LEFT: 
        if( m_gridPosition.X == 1 ) m_iHealth=0;
        else 
            occ = grid.GetTile( m_gridPosition.X-1, m_gridPosition.Y+1 ).GetOccupier(); 
        break;

    case DOWN: 
        if( m_gridPosition.Y < grid.tilesY-1 )
            occ = grid.GetTile( m_gridPosition.X, m_gridPosition.Y+1 ).GetOccupier(); 
        break;

    case RIGHT: 
        if( m_gridPosition.X == grid.tilesX-2 ) m_iHealth=0;
        else 
            occ = grid.GetTile( m_gridPosition.X+1, m_gridPosition.Y+1 ).GetOccupier(); 
        break;
    }

    bool reset = false;
    if( occ )
    {
        if( mgr.IsOfType("end", occ->GetType()) ) reset = true;
        else if( mgr.IsOfType("wall", occ->GetType()) ) m_iHealth = 0;
        else if( mgr.IsOfType("big_debris", occ->GetType()) ) m_iHealth = 0;
        else if( mgr.IsOfType("small_debris", occ->GetType()) ) m_iHealth -= 50;
        else if( mgr.IsOfType("water", occ->GetType()) ) m_iHealth -= 10;
    }

    m_iHealth = ( m_iHealth < 0 ) ? 0 : m_iHealth;
    double fitness = 1.0 - (double)( health - m_iHealth ) / m_desc->GetMaxHealth();
    if( fitness < 0.0001 || m_iHealth == 0 )
    {
        return 0.0;
    }
    else if( reset ) 
    {
        return 1.1;
    }

#ifdef _PUNISH_DIAGONAL_
    switch(action)
    {
    case LEFT: fitness *= 0.95; break;
    case RIGHT: fitness *= 0.95; break;
    }
#endif

    m_action = (int)action;
    Update();

    return fitness;
}

// ----------------------------------------------------------------------------
void CUnit::Set( int x, int y )
{
    assert(x>=0 && x<Grid::Get().tilesX);
    assert(y>=0 && y<Grid::Get().tilesY);

    IObject::Set(x, y);

    // get the tile at that point and update it
    m_tile = & (Grid::Get().GetTile(x, y));
    m_tile->SetOccupier(this);
}

// ----------------------------------------------------------------------------
void CUnit::CloneFrom( IObject *obj )
{
    IObject::CloneFrom(obj);

    CUnit *dummy = (CUnit*)obj;

    m_iHealth = dummy->GetHealth();
    m_iSpeed = dummy->GetSpeed();
    m_desc = dummy->GetDesc();
}

// ----------------------------------------------------------------------------
void CUnit::SetDesc( CUnitDesc *desc ) 
{ 
    assert(desc!=0);

    m_desc = desc;
    //Reset();
}

// ----------------------------------------------------------------------------
void CUnit::Reset()
{
    m_iHealth = m_desc->GetMaxHealth();
    m_iSpeed = 0;
    m_direction.X = 0.0f;
    m_direction.Y = 1.0f;

    int X = RandInt(1, Grid::Get().tilesX-2);

    Set(X, m_initialGridPos.Y);
    m_previousGridPos.X = X;
    m_previousGridPos.Y = m_initialGridPos.Y;
}

// ----------------------------------------------------------------------------
void CUnit::Render() const
{
    DrawRectangle( m_desc->GetColor(), m_realPosition );
    
    Grid& grid = Grid::Get();

    if( debug.drawFOV )
    {
        position2di ll(
            grid.tilesize.Width, 
            m_realPosition.Y + grid.tilesize.Height
            );
        position2di hr(
            grid.width - grid.tilesize.Width, 
            m_realPosition.Y + grid.tilesize.Height + m_desc->GetFOV() * grid.tilesize.Height
            );

        rts::video->draw2DRectangle(
            SColor(250,225,225,225),
            rect<s32>( ll.X, ll.Y, hr.X, hr.Y )
            );
    }

    if( debug.drawDir )
    {
        position2di prevPos(
            m_previousGridPos.X * grid.tilesize.Width,
            m_previousGridPos.Y * grid.tilesize.Height
            );

        DrawRectangle( *(MgrColor::Get().GetData("green")), prevPos );
    }
    
    if( debug.drawId ) SetRenderID(GetID());
    else
        SetRenderID(0);

}

// ----------------------------------------------------------------------------
void CUnit::Update()
{
    if(m_action==NEUTRAL) return;
    
    m_previousGridPos = m_gridPosition;
    switch((E_ACTION)m_action)
    {
    case LEFT: IObject::Set( m_gridPosition.X-1, m_gridPosition.Y+1 ); break;
    case DOWN: IObject::Set( m_gridPosition.X, m_gridPosition.Y+1 ); break;
    case RIGHT: IObject::Set( m_gridPosition.X+1, m_gridPosition.Y+1 ); break;
    }

    m_action = NEUTRAL;

    _log_2n("Unit moved to: ", m_gridPosition.X, ", ", m_gridPosition.Y);
}

// ----------------------------------------------------------------------------
void CUnit::Update(float dt)
{
    float dx = m_iSpeed * m_direction.X * dt;
    float dy = m_iSpeed * m_direction.Y * dt;
    //_log_2n("dX=", dx, ", dY=", dy);

    static float accumX = 0.0f;
    static float accumY = 0.0f;

    accumX += dx;
    accumY += dy;
    //_log_2n("accumX=", accumX, ", accumY=", accumY);

    int iacX = (int) accumX;
    int iacY = (int) accumY;

    m_realPosition.X += iacX;
    m_realPosition.Y += iacY;
    //_log_2n("m_realPosition.X=", m_realPosition.X, ", m_realPosition.Y=", m_realPosition.Y);

    accumX -= (float) iacX;
    accumY -= (float) iacY;
}

// ----------------------------------------------------------------------------
void CUnit::Write( stringw &out ) const
{
    IObject::Write(out);
    //m_desc->Write(out);
    _OUT(L"Health: ", m_iHealth);
    //_OUT(L"Speed: ", m_iSpeed);
}