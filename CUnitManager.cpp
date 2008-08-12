#include "CUnitManager.h"
#include "CColorManager.h"
#include "CGunManager.h"
#include "./CObjectTypeManager.h"

// ----------------------------------------------------------------------------
void CUnitManager::Update(float dt)
{
    UnitArray::iterator i = m_units.begin();
    for(; i != m_units.end(); ++i )
    {
        CUnit *obj = *i;
        if(obj->IsActive()) obj->Update(dt);
    }
}

// ----------------------------------------------------------------------------
void CUnitManager::ReadXMLData( IrrXMLReader *xml )
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
                const char *type  = xml->getAttributeValue("type");
                const char *name  = xml->getAttributeValue("name");
                const char *gun   = xml->getAttributeValue("gun");
                const char *color = xml->getAttributeValue("color");
                int speed         = xml->getAttributeValueAsInt("max_speed");
                int speed_ratio   = xml->getAttributeValueAsInt("speed_ratio");
                int health        = xml->getAttributeValueAsInt("max_health");
                int fov           = xml->getAttributeValueAsInt("fov");
                int attack        = xml->getAttributeValueAsInt("attack");
                int defence       = xml->getAttributeValueAsInt("defence");

                MgrObjectType::Get().AddType(type);

                // add gun to gun description manager database
                CUnitDesc *unitDesc = m_data.Get( index );
                unitDesc->SetName(name);
                unitDesc->SetGunType(gun);
                unitDesc->SetMaxSpeed(speed);
                unitDesc->SetSpeedRatio(speed_ratio);
                unitDesc->SetMaxHealth(health);
                unitDesc->SetFOV(fov);
                unitDesc->SetAttack(attack);
                unitDesc->SetDefence(defence);
                unitDesc->SetColor( *(MgrColor::Get().GetData(color)) );

                m_data.Insert( type, index++ );
            }
            break;
        }
    } // while

    m_unitPool.create(iPool);

} // ReadXMLData

// ----------------------------------------------------------------------------
CUnit* CUnitManager::CreateUnit( const char *type )
{
    CUnitDesc *unitDesc = GetData(type);
    CGunDesc *gunDesc = MgrGun::Get().GetData( unitDesc->GetGunType() );

    CUnit *new_unit = m_unitPool.get();
    new_unit->SetDesc(unitDesc);
    new_unit->SetGunDesc(gunDesc);

    m_units.push_back(new_unit);
    return new_unit;
}