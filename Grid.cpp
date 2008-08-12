#include "Grid.h"

// ----------------------------------------------------------------------------
Grid::Grid():
m_tiles(0)
{}

// ----------------------------------------------------------------------------
Grid::~Grid()
{
    delete [] m_tiles;
}

// ----------------------------------------------------------------------------
void Grid::InitTiles()
{
    assert(m_tiles==0);

    m_tiles = new CTile[ tilesX * tilesY ];
}

// ----------------------------------------------------------------------------
bool Grid::IsFree( int x, int y ) const 
{
    if(x<0 || x>=tilesX) return false;
    if(y<0 || x>=tilesY) return false;

    if( m_tiles[x + y * tilesX].GetOccupier() )
        return false;

   return true;
}

// ----------------------------------------------------------------------------
bool Grid::IsFree( const position2di& pos ) const
{
    return IsFree(pos.X, pos.Y);
}

// ----------------------------------------------------------------------------
bool Grid::IsFree( const CTile& tile ) const
{
    return IsFree( tile.GetGridPosition() );
}

// ----------------------------------------------------------------------------
CTile& Grid::BelongsTo( const position2di& pos )
{
    int x = pos.X / tilesize.Width;
    int y = pos.Y / tilesize.Height;

    return _GetTile(x, y);
}