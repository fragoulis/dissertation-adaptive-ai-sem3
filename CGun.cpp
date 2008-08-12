#include "CGun.h"
#include "CGunDesc.h"
#include <cassert>

#define _OUT(label, value) out += label; out += value; out += "\n";

// ----------------------------------------------------------------------------
void CGun::Write( stringw &out ) const
{
    m_desc->Write(out);
    _OUT(L"Ammo: ", m_iAmmo);
}

// ----------------------------------------------------------------------------
void CGun::Reset()
{
    m_iAmmo = m_desc->GetMaxAmmo();
}

// ----------------------------------------------------------------------------
void CGun::SetDesc( CGunDesc *desc ) 
{ 
    assert(desc!=0);

    m_desc = desc;
    Reset();
}

// ----------------------------------------------------------------------------
void CGun::Shoot()
{
    if( m_iAmmo <= 0 ) return;

    // Check with the rate of fire
    if( m_iROFTimer == m_desc->GetROF() )
    {
        // Update ammunition
        m_iAmmo--;

        // Reset ROF timer
        m_iROFTimer = 0;
    }
    else
        m_iROFTimer++;
}