#include "IObject.h"
#include "CTile.h"
#include "CObjectTypeManager.h"
#include <cassert>

#define _OUT(label, value) out += label; out += value; out += "\n";

// ----------------------------------------------------------------------------
int IObject::CURRENT_ID = 1;

// ----------------------------------------------------------------------------
IObject::IObject():
m_id(CURRENT_ID++),
m_realPosition(position2di(0, 0)),
m_gridPosition(position2di(0, 0)),
m_tile(0),
m_iType(0)
{}

// ----------------------------------------------------------------------------
void IObject::Set( int x, int y )
{
    // update the position on the grid
    m_gridPosition.X = x;
    m_gridPosition.Y = y;

    // update the position on the window
    m_realPosition.X = x * Tilesize.Width;
    m_realPosition.Y = y * Tilesize.Height;
}

// ----------------------------------------------------------------------------
void IObject::CloneFrom( IObject *obj )
{
    m_iType = obj->GetType();
    m_realPosition = obj->GetRealPosition();
    m_gridPosition = obj->GetGridPosition();
    m_bActive = obj->IsActive();
}

// ----------------------------------------------------------------------------
position2di IObject::GetCenter() const
{
    return position2di( 
        m_realPosition.X + Tilesize.Width / 2, 
        m_realPosition.Y + Tilesize.Height / 2 
        );
}

// ----------------------------------------------------------------------------
void IObject::SetRenderID( int num ) const
{
    if(num==0) {
        m_text->setText(0);
        return;
    }

    stringw str;
    str += num;
    m_text->setText(str.c_str());

    int x = m_realPosition.X + 1;
    int y = m_realPosition.Y + 1;
    m_text->setRelativePosition(
        rect<s32>(x, y, x+10, y+10)
        );
}

// ----------------------------------------------------------------------------
void IObject::SetTile( CTile *tile )
{
    assert(tile->GetOccupier()==0);

    m_tile->SetOccupier(0);
    tile->SetOccupier(this);
    m_tile = tile;
    
    m_gridPosition.X = m_tile->GetGridPosition().X;
    m_gridPosition.Y = m_tile->GetGridPosition().Y;
}

// ----------------------------------------------------------------------------
void IObject::Write( stringw &out ) const
{
    out += "[";
    out += m_id;
    out += "] ";
    _OUT(L"Type: ", MgrObjectType::Get().GetTypeFromInt(m_iType));
}