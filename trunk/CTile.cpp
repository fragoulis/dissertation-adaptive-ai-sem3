#include "CTile.h"
#include "Grid.h"

#define _OUT(label, value) out += label; out += value; out += "\n";
//#define _USE_DIAGONAL_PATH

// ----------------------------------------------------------------------------
CTile::CTile():
m_color(SColor(250, 250, 0, 0)),
m_occupier(0)
{
}

// ----------------------------------------------------------------------------
CTile::CTile( int x, int y ):
m_color(SColor(250, 250, 0, 0)),
m_occupier(0)
{
    Set(x, y);
}

// ----------------------------------------------------------------------------
void CTile::Render() const
{
    DrawRectangle( m_color, m_realPosition );
}