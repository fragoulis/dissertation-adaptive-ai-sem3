#include "CColorManager.h"

void CColorManager::ReadXMLData( IrrXMLReader *xml )
{
    // read root tag first
    xml->read(); 
    int iNum = xml->getAttributeValueAsInt("count");

    // allocate pool memory for color objects
    m_data.Create(iNum); 

    int index = 0;
    while(xml->read()) // move to read children colors
    {
        switch(xml->getNodeType())
        {
        case EXN_ELEMENT:
            {
                // read color attributes from file
                const char *color_name = xml->getAttributeValue("name");
                u32 red   = xml->getAttributeValueAsInt("red");
                u32 green = xml->getAttributeValueAsInt("green");
                u32 blue  = xml->getAttributeValueAsInt("blue");
                u32 alpha = xml->getAttributeValueAsInt("alpha");

                // add color to color manager database
                SColor *col = m_data.Get( index );
                col->set( alpha, red, green, blue );

                m_data.Insert( color_name, index++ );
            }
            break;
        }
    }
}