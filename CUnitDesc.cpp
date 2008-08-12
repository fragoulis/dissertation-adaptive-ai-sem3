#include "CUnitDesc.h"

#define _OUT(label, value) out += label; out += value; out += "\n";

// ----------------------------------------------------------------------------
void CUnitDesc::Write( stringw &out ) const
{
    _OUT(L"Unit: ",      m_sName);
    _OUT(L"FOV: ",       m_iFOV);
    _OUT(L"MaxSpeed: ",  m_iMaxSpeed);
    _OUT(L"MaxHealth: ", m_iMaxHealth);
    _OUT(L"Attack: ",    m_iAttack);
    _OUT(L"Defence: ",   m_iDefence);
}