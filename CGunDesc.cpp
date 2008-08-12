#include "CGunDesc.h"

#define _OUT(label, value) out += label; out += value; out += "\n";

// ----------------------------------------------------------------------------
void CGunDesc::Write( stringw &out ) const 
{
    _OUT(L"Gun: ",            m_sName);
    _OUT(L"Power: ",          m_iPower);
    _OUT(L"MaxAmmo: ",        m_iMaxAmmo);
    _OUT(L"Rate of fire: ",   m_iROF);
    _OUT(L"Area of effect: ", m_iAOE);
}