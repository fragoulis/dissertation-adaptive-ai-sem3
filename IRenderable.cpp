#include "IRenderable.h"

// ----------------------------------------------------------------------------
dimension2di IRenderable::Tilesize;

// ----------------------------------------------------------------------------
IRenderable::IRenderable():
m_bActive(true)
{
    m_text = rts::gui->addStaticText(0, rect<s32>(0,0,Tilesize.Width,Tilesize.Height));
}

// ----------------------------------------------------------------------------
void IRenderable::DrawRectangle( 
                                const SColor &color, 
                                const position2di &position ) const
{
    rts::video->draw2DRectangle( 
        color, 
        core::rect<s32>( position, Tilesize ) 
        );
}

// ----------------------------------------------------------------------------
void IRenderable::DrawRectangle( 
                                const SColor &color, 
                                const position2di &position, 
                                const dimension2di &size ) const
{
    rts::video->draw2DRectangle( 
        color, 
        core::rect<s32>( position, size ) 
        );
}

// ----------------------------------------------------------------------------
void IRenderable::DrawLine( 
                           const position2di &start, 
                           const position2di &end, 
                           const SColor &color ) const
{
    rts::video->draw2DLine( start, end, color );
}