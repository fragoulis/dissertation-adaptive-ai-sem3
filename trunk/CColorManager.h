#pragma once
#include "IManager.h"
#include <SColor.h>
using irr::video::SColor;

class CColorManager : public IManager<SColor, CColorManager>
{
private:
    virtual void ReadXMLData( IrrXMLReader *xml );
};

typedef CColorManager MgrColor;