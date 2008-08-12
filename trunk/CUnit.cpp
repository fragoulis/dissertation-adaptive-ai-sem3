#include "CUnit.h"
#include "CTeam.h"
#include "Grid.h"
#include "logger.h"
#include "CObjectTypeManager.h"
#include "Random.h"

#define _OUT(label, value) out += label; out += value; out += "\n";

#define _PUNISH_DIAGONAL_

// ----------------------------------------------------------------------------
_debug CUnit::debug;

// ----------------------------------------------------------------------------
CUnit::CUnit()
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
        else if( mgr.IsOfType("small_debris", occ->GetType()) ) m_iHealth -= 50;
        else if( mgr.IsOfType("big_debris", occ->GetType()) ) m_iHealth = 0;
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

    switch(action)
    {
    case LEFT: IObject::Set( m_gridPosition.X-1, m_gridPosition.Y+1 ); break;
    case DOWN: IObject::Set( m_gridPosition.X, m_gridPosition.Y+1 ); break;
    case RIGHT: IObject::Set( m_gridPosition.X+1, m_gridPosition.Y+1 ); break;
    }

#ifdef _PUNISH_DIAGONAL_
    switch(action)
    {
    case LEFT: fitness *= 0.95; break;
    case RIGHT: fitness *= 0.95; break;
    }
#endif

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
    //m_gun = obj->GetRows();
    m_desc = dummy->GetDesc();
}

// ----------------------------------------------------------------------------
void CUnit::SetDesc( CUnitDesc *desc ) 
{ 
    assert(desc!=0);

    m_desc = desc;
    Reset();
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
}

// ----------------------------------------------------------------------------
void CUnit::Render() const
{
    rts::video->draw2DRectangle( 
        core::rect<s32>( m_realPosition, Tilesize ), 
        m_desc->GetColor(), 
        m_desc->GetColor(),
        m_desc->GetColor(),
        m_team->GetColor()
        );

    if( debug.drawFOV )
    {
        position2di pos = GetCenter();
        rts::video->draw2DPolygon( 
            pos, 
            (float)m_desc->GetFOV() * Tilesize.Height, 
            SColor(250,200,200,200), 
            40 
            );
    }

    if( debug.drawDir )
    {
        position2di dir = position2di(
            int(m_direction.X*Tilesize.Width), 
            int(m_direction.Y*Tilesize.Height)
            );
        position2di pos = GetCenter();
        rts::video->draw2DLine( 
            pos, 
            pos + dir,
            SColor(250,0,0,0) 
            );
    }
    
    if( debug.drawId ) SetRenderID(GetID());
    else
        SetRenderID(0);

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
    m_desc->Write(out);
    _OUT(L"Health: ", m_iHealth);
    _OUT(L"Speed: ", m_iSpeed);
    m_gun.Write(out);
}