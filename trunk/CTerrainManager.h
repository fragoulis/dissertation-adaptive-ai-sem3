#pragma once
#include "IManager.h"
#include "CTerrainObject.h"
#include "TFreelist.h"
#include <vector>

class CTerrainManager : public IManager<CTerrainObject, CTerrainManager>
{
typedef std::vector<CTerrainObject*> UnitArray;

    //! A memory pool for units.
    TFreelist<CTerrainObject> m_objPool;
    
    //! An array of pointer to allocated units.
    UnitArray m_objects;

public:
    CTerrainObject* CreateObject( const char *type );

private:
    virtual void ReadXMLData( IrrXMLReader *xml );
};

typedef CTerrainManager MgrTerrain;