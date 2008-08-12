#include "CTeamManager.h"
#include "CColorManager.h"

void CTeamManager::ReadXMLData( IrrXMLReader *xml )
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
                const char *sName = xml->getAttributeValue("name");
                const char *sColor = xml->getAttributeValue("color");

                CTeam *team = m_data.Get( index );
                team->SetName(sName);
                team->SetColor( *(MgrColor::Get().GetData(sColor)) );

                m_data.Insert(sName, index++);
            }
            break;
        }
    }
}