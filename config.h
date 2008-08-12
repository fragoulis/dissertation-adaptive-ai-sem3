#pragma once
#include <dimension2d.h>
#include <map>
#include <string>

class config
{
public:
    void init( const char *filename );

    bool fullscreen;
    irr::core::dimension2di wndSize;
    std::string active_map;

    typedef std::map< std::string, std::string > Paths;
    Paths paths;
};