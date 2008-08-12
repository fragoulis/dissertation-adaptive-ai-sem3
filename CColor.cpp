#include "CColor.h"
#include <iostream>

void CColor::Write( ostream &out ) const 
{
    out << "Color(" << GetRed() << ", " << GetGreen() << ", " << GetBlue() << ")" << std::endl;
}