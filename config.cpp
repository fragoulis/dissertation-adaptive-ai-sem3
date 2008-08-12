#include "config.h"
#include <cassert>
#include <cstring>
#include <irrXML.h>
using namespace irr;
using namespace io;

#pragma warning(disable:4800)
#include <iostream>

void config::init( const char *filename )
{
    IrrXMLReader *xml = createIrrXMLReader(filename);
    assert(xml);

    // read root tag's variables
    xml->read();
    fullscreen   = xml->getAttributeValueAsInt("fullscreen");
    active_map   = xml->getAttributeValue("map");

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
                else

                // TODO: Fix this thing. The paths do not get inserted in the map.
                if( !strcmp( "paths", xml->getNodeName() ) )
                {
                    int count = xml->getAttributeValueAsInt("count");
                    for( int i=0; i<count; i++ )
                    {
                        bool ended = false;
                        while(xml->read() && !ended)
                        {
                            if(xml->getNodeType() == EXN_ELEMENT)
                            {
                                paths.insert( 
                                    std::make_pair( 
                                        std::string(xml->getNodeName()), 
                                        std::string(xml->getAttributeValue("path")) 
                                    )
                                );
                                ended = true;
                            } // if
                        } // while
                    } // for 
                } // if
            }
            break;
        }
    }

    delete xml;
}