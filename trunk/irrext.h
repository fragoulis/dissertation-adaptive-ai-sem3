#pragma once

// Main engine file
#include <irrlicht.h>

// Make use of the engine's namespaces for our easiness
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//! Helper structure that will give global access to irrlicht interfaces.
class rts
{
public:
    static IVideoDriver *video;
    static ISceneManager *scene;
    static IGUIEnvironment *gui;

    static IGUIStaticText* addStaticText( const wchar_t *text, rect<s32> &pos );
};