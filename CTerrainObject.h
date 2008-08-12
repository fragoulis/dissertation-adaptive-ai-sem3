#pragma once
#include "IObject.h"

class CTerrainObject : public IObject
{
private:
    //! The amount of cover it provides.
    //! Value range from 0 to 100.
    int m_iCover;

    //! The amount of visibility it provides.
    //! Value range from 0 to 100.
    int m_iVisibility;

    //! The width of the object in tiles.
    int m_iCols;

    //! The height of the object in tiles.
    int m_iRows;

    //! General color flag.
    SColor m_color;

public:
    //! Constructor.
    CTerrainObject();

    //! Returns the cover value.
    int GetCover() const { return m_iCover; }

    //! Returns the visibility value.
    int GetVisibility() const { return m_iVisibility; }

    //! Returns the number of columns the object occupies.
    int GetCols() const { return m_iCols; }

    //! Returns the number of rows the object occupies.
    int GetRows() const { return m_iRows; }

    //! Return the color.
    const SColor& GetColor() const { return m_color; }

    //! Sets the cover value.
    void SetCover( int cover ) { m_iCover = cover; }

    //! Sets the visibility value.
    void SetVisibility( int visibility ) { m_iVisibility = visibility; }

    //! Sets the number of columns the object occupies.
    void SetCols( int cols ) { m_iCols = cols; }

    //! Sets the number of rows the object occupies.
    void SetRows( int rows ) { m_iRows = rows; }

    //! Sets the color.
    void SetColor( const SColor &color ) { m_color = color; }

    //! Copy's the attributes of another object.
    virtual void CloneFrom( IObject *obj );

    //! Renders the object.
    virtual void Render() const;

    //! Updates the object
    virtual void Update(float dt) {}

    //! Serializes the object.
    virtual void Write( stringw &out ) const;
};