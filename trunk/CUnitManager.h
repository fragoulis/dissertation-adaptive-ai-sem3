#pragma once
#include "IManager.h"
#include "TFreelist.h"
#include "CUnitDesc.h"
#include "CUnit.h"
#include <vector>

class CUnitManager : public IManager<CUnitDesc, CUnitManager>
{
private:
    typedef std::vector<CUnit*> UnitArray;

    //! A memory pool for units.
    TFreelist<CUnit> m_unitPool;
    
    //! An array of pointer to allocated units.
    UnitArray m_units;

public:
    //! Updates the units.
    void Update(float dt);

    //! Creates a new unit and returns a pointer to it.
    CUnit* CreateUnit( const char *type );

    //! Returns a unit from the array.
    CUnit* GetUnit( size_t index ) { return m_units[index]; }

    //! Returns a const reference of the first unit from the array.
    const CUnit& GetActor() const { return * ( m_units[0] ); }

    //! Returns a non-const reference of the first unit from the array.
    CUnit& GetActor() { return * ( m_units[0] ); }

    //! Returns the numbder of units in the array.
    int Size() const { return (int)m_units.size(); }

private:
    //! Initializes all unit types from XML file.
    virtual void ReadXMLData( IrrXMLReader *xml );
};

typedef CUnitManager MgrUnit;