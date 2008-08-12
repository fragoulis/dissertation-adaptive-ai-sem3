#pragma once
#include <vector>
#include "Singleton.h"
#include "CTile.h"

class Grid : public Singleton<Grid>
{
public:
    //typedef std::vector<CTile> Tiles;
    
    //! The width of the grid map in pixels.
    int width;
    
    //! The height of the grid map in pixels.
    int height;
    
    //! The number of tiles along the X-axis.
    int tilesX;

    //! The number of tiles along the Y-axis.
    int tilesY;
    
    //! The individual tile's dimensions.
    dimension2di tilesize;

    //! Returns true if chosen tile is unoccupied.
    bool IsFree( int x, int y ) const;

    //! Returns true if chosen tile is unoccupied.
    bool IsFree( const position2di& pos ) const;

    //! Returns true if chosen tile is unoccupied.
    bool IsFree( const CTile& tile ) const;

    //! Returns a const reference of a tile.
    const CTile& GetTile( int index ) const { return m_tiles[index]; }

    //! Returns a const reference of a tile.
    const CTile& GetTile( int x, int y ) const { return _GetTile(x, y); }

    //! Returns a const reference of a tile.
    const CTile& GetTile( const position2di& pos ) const { return _GetTile(pos.X, pos.Y); }

    //! Returns a non-const reference of a tile.
    CTile& GetTile( int x, int y ) { return _GetTile(x, y); }

    //! Returns the tile in which a position belogns to.
    CTile& BelongsTo( const position2di& pos );

    //! Sets a tile's grid and real position
    void SetTile( int x, int y ) { _GetTile(x, y).Set(x, y); }

    //! Sets the occupier of the tile.
    void SetOccupier( int x, int y, IObject *occ ) { _GetTile(x, y).SetOccupier(occ); }

    //! Allocates space for the tiles.
    void InitTiles();

private:
    //! Constructor.
    Grid();

    //! Destructor.
    ~Grid();

    friend Singleton<Grid>;

    //! Returns a reference of a tile.
    CTile& _GetTile( int x, int y ) { return m_tiles[x + y * tilesX]; }

    //! Returns a const reference of a tile.
    const CTile& _GetTile( int x, int y ) const { return m_tiles[x + y * tilesX]; }

    //! The array of tiles.
    CTile *m_tiles;
};