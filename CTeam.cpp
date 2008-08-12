#include "CTeam.h"
#include "CUnit.h"

void CTeam::Insert( CUnit *unit ) 
{
    unit->SetTeam(this);
    m_units.push_back(unit); 
}
