#include "CGunManager.h"

void CGunManager::ReadXMLData( IrrXMLReader *xml )
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
                const char *type = xml->getAttributeValue("type");
                const char *name = xml->getAttributeValue("name");
                int power        = xml->getAttributeValueAsInt("power");
                int max_ammo     = xml->getAttributeValueAsInt("max_ammo");
                int rof          = xml->getAttributeValueAsInt("rof");
                int aoe          = xml->getAttributeValueAsInt("aoe");

                // add gun to gun description manager database
                CGunDesc *gunDesc = m_data.Get( index );
                gunDesc->SetName(name);
                gunDesc->SetPower(power);
                gunDesc->SetMaxAmmo(max_ammo);
                gunDesc->SetROF(rof);
                gunDesc->SetAOE(aoe);

                m_data.Insert( type, index++ );
            }
            break;
        }
    }
}