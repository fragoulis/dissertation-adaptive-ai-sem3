#pragma once
#include <dimension2d.h>
#include <string>

class config
{
public:
    static void init( const char *filename );

    static bool fullscreen;
    static irr::core::dimension2di wndSize;
    static std::string active_map;
};