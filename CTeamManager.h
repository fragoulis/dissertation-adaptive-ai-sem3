#pragma once
#include "IManager.h"
#include "CTeam.h"

class CTeamManager : public IManager<CTeam, CTeamManager>
{
private:
    virtual void ReadXMLData( IrrXMLReader *xml );
}; 

typedef CTeamManager MgrTeam;