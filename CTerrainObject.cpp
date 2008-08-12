#include "CTerrainObject.h"
#include "Grid.h"

#define _OUT(label, value) out += label; out += value; out += "\n";

// ----------------------------------------------------------------------------
CTerrainObject::CTerrainObject():
m_color(SColor(250, 0, 0, 0))
{}

// ----------------------------------------------------------------------------
void CTerrainObject::CloneFrom( IObject *obj )
{
    IObject::CloneFrom(obj);

    CTerrainObject *dummy = (CTerrainObject*)obj;

    m_iCover = dummy->GetCover();
    m_iVisibility = dummy->GetVisibility();
    m_iRows = dummy->GetRows();
    m_iCols = dummy->GetCols();
    m_color = dummy->GetColor();
}

// ----------------------------------------------------------------------------
void CTerrainObject::Render() const 
{
    for( int row=0; row<m_iRows; row++ )
    {
        int Y = m_realPosition.Y + row * Grid::Get().tilesize.Height;
        for( int col=0; col<m_iCols; col++ )
        {
            int X = m_realPosition.X + col * Grid::Get().tilesize.Width;

            DrawRectangle( m_color, position2di(X, Y) );
        }
    }
}

// ----------------------------------------------------------------------------
void CTerrainObject::Write( stringw &out ) const
{
    _OUT(L"Cover: ", m_iCover);
    _OUT(L"Visibility: ", m_iVisibility);
}
