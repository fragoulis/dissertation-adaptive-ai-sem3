#pragma once
#include <cassert>
#include <irrString.h>
using irr::core::stringw;

class CGunDesc;

class CGun
{
private:
    //! The current ammo availabe for the gun.
    int m_iAmmo;

    //! Rate of fire timer.
    int m_iROFTimer;

    //! Common gun attributes shared among all guns.
    //! This is what distinguishes guns from each other.
    CGunDesc *m_desc;

public:
    CGun(): m_desc(0), m_iROFTimer(0) {}
    CGun( CGunDesc *desc ): m_iROFTimer(0) { SetDesc( desc ); }

    //! Returns the current ammunition.
    int GetAmmo() const { return m_iAmmo; }
    
    //! Initializes the gun.
    void SetDesc( CGunDesc *desc );

    //! Shoots the gun.
    //! Checks the rate of fire and updates the gun's attributes.
    void Shoot();

    //! Resets ammunition.
    void Reset();

    void Write( stringw &out ) const;
};
    