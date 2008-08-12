#pragma once
#include "IManager.h"
#include "CGunDesc.h"

class CGunManager : public IManager<CGunDesc, CGunManager>
{
private:
    virtual void ReadXMLData( IrrXMLReader *xml );
};

typedef CGunManager MgrGun;