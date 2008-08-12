#include "CTerrainManager.h"
#include "CColorManager.h"
#include "./CObjectTypeManager.h"

void CTerrainManager::ReadXMLData( IrrXMLReader *xml )
{
    // read root tag first
    xml->read(); 
    int iNum    = xml->getAttributeValueAsInt("count");
    int iPool   = xml->getAttributeValueAsInt("pool");

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
                const char *sType  = xml->getAttributeValue("type");
                const char *sColor = xml->getAttributeValue("color");
                int cover       = xml->getAttributeValueAsInt("cover");
                int visibility  = xml->getAttributeValueAsInt("visibility");
                int cols        = xml->getAttributeValueAsInt("cols");
                int rows        = xml->getAttributeValueAsInt("rows");
                
                int iType = MgrObjectType::Get().AddType(sType);

                // add color to color manager database
                CTerrainObject *obj = m_data.Get( index );
                obj->SetType(iType);
                obj->SetCover(cover);
                obj->SetVisibility(visibility);
                obj->SetCols(cols);
                obj->SetRows(rows);
                obj->SetColor( *(MgrColor::Get().GetData(sColor)) );

                m_data.Insert( sType, index++ );
            }
            break;
        }
    } // while

    m_objPool.create(iPool);

} // ReadXMLData

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
CTerrainObject* CTerrainManager::CreateObject( const char *type )
{
    // Retrieve object type descrition
    CTerrainObject *clone = GetData(type);

    // Allocate and initialize object
    CTerrainObject *new_obj = m_objPool.get();
    new_obj->CloneFrom( clone );

    m_objects.push_back(new_obj);

    return new_obj;
}