#include "irrext.h"

IVideoDriver*       rts::video;
ISceneManager*      rts::scene;
IGUIEnvironment*    rts::gui;

IGUIStaticText* rts::addStaticText( const wchar_t *text, rect<s32> &pos )
{
    return gui->addStaticText(
        text, 
        rect<s32>(
            pos.UpperLeftCorner.X,
            pos.UpperLeftCorner.Y,
            pos.UpperLeftCorner.X + pos.LowerRightCorner.X,
            pos.UpperLeftCorner.Y + pos.LowerRightCorner.Y
            )
            );
}