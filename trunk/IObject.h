#pragma once
#include "position2d.h"
#include "IRenderable.h"

class CTile;

class IObject : public IRenderable
{
protected:
    //! The type of object.
    int m_iType;

    //! Position in window ccordinates.
    position2di m_realPosition;

    //! Position in grid coordinates.
    position2di m_gridPosition;

    //! The tile in which the object it currently on.
    //! For terrain objects occupying more than one tile this is the upper left tile.
    CTile *m_tile;

private:
    //! The object's unique id.
    //! This is passed during initialization as serial number.
    int m_id;

    //! The id counter.
    //! Every time a new object is created this number increases by one.
    static int CURRENT_ID;

public:
    //! Constructor.
    IObject();

    //! Virtual destructor.
    virtual ~IObject() {}

    //! Sets a string type.
    void SetType( int type ) { m_iType = type; }

    //! Sets an object's grid and real position
    virtual void Set( int x, int y );

    //! Sets the real window position.
    void SetRealPosition( const position2di &pos ) { m_realPosition = pos; }

    //! Sets the grid position.
    void SetGridPosition( const position2di &pos ) { m_gridPosition = pos; }

    //! Sets the tile.
    void SetTile( CTile *tile );

    //! Returns the id.
    int GetID() const { return m_id; }

    //! Returns the string type.
    int GetType() const { return m_iType; }

    //! Returns the real window position.
    const position2di& GetRealPosition() const { return m_realPosition; }

    //! Returns the grid position.
    const position2di& GetGridPosition() const { return m_gridPosition; }

    //! Returns the tile which the unit occupies.
    CTile* GetTile() { return m_tile; }

    //! Returns the center position.
    position2di GetCenter() const;

    //! Renders a number at the center of the tile.
    void SetRenderID( int num ) const;

    //! Override this to update the object.
    virtual void Update(float dt) = 0;

    //! Copy's the attributes of another object.
    virtual void CloneFrom( IObject *obj );

    //! Serializes the object.
    virtual void Write( stringw &out ) const;
};