#pragma once
#include "irrext.h"

class IRenderable
{
protected:
    //! Active flag.
    bool m_bActive;

    //! Set the text to appear at the center of the tile.
    IGUIStaticText* m_text;

public:
    //! Constructor.
    IRenderable();

    //! Virtual destructor.
    virtual ~IRenderable() {}

    //! Override this to render the object.
    virtual void Render() const = 0;

    //! Activates the object.
    void Activate() { m_bActive = true; }

    //! Deactivates the object.
    void Deactivate() { m_bActive = false; }

    //! Returns if object is active.
    bool IsActive() const { return m_bActive; }

    //! Sets the text.
    void SetText( const wchar_t *text ) { m_text->setText(text); }

public:
    //! Draws a rectangle.
    //! Used to ease the drawing process.
    void DrawRectangle( const SColor &color, const position2di &position ) const;

    //! Draws a rectangle without using the default tilesize.
    //! Used to ease the drawing process.
    void DrawRectangle( const SColor &color, const position2di &position, const dimension2di &size ) const;

    //! Draws a 2d line.
    void DrawLine( const position2di &start, const position2di &end, const SColor &color ) const;
};