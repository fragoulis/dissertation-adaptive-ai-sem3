#include "config.h"
#include <cassert>
#include <cstring>
#include <irrXML.h>
using namespace irr;
using namespace io;

#pragma warning(disable:4800)
#include <iostream>

// ----------------------------------------------------------------------------
bool config::fullscreen;
    
// ----------------------------------------------------------------------------
std::string config::active_map;

// ----------------------------------------------------------------------------
irr::core::dimension2di config::wndSize;

// ----------------------------------------------------------------------------
int config::max_tries;

// ----------------------------------------------------------------------------
void config::init( const char *filename )
{
    IrrXMLReader *xml = createIrrXMLReader(filename);
    assert(xml);

    // read root tag's variables
    xml->read();
    fullscreen   = xml->getAttributeValueAsInt("fullscreen");
    active_map   = xml->getAttributeValue("map");
    max_tries    = xml->getAttributeValueAsInt("max_tries");

    // read consiquent children tags
    while(xml->read())
    {
        switch(xml->getNodeType())
        {
        case EXN_ELEMENT:
            {
                if( !strcmp( "window", xml->getNodeName() ) )
                {
                    wndSize.Width = xml->getAttributeValueAsInt("width");
                    wndSize.Height = xml->getAttributeValueAsInt("height");
                }
            }
            break;
        }
    }

    delete xml;
}