#pragma once
#include <irrString.h>
using irr::core::stringw;

class CGunDesc
{
private:
    //! The printable name of the gun.
    stringw m_sName;

    //! Maximum ammunition.
    int m_iMaxAmmo;

    //! The rate of fire.
    //! Updates before the gun can fire again.
    int m_iROF;

    //! Maximum damage inflicted.
    int m_iPower;

    //! The area of effect measured in tiles.
    int m_iAOE;

public:
    //! Sets the descriptor's screen name.
    void SetName( const char *name )    { m_sName = name; }

    //! Sets the power.
    void SetPower( int power )          { m_iPower = power; }

    //! Sets the maximum ammo.
    void SetMaxAmmo( int ammo )         { m_iMaxAmmo = ammo; }

    //! Sets the rate of fire.
    void SetROF( int rof )              { m_iROF = rof; }

    //! Sets the area of effect.
    void SetAOE( int aoe )              { m_iAOE = aoe; }

    //! Returns the power.
    int GetPower() const    { return m_iPower; }

    //! Returns the maximum ammo.
    int GetMaxAmmo() const  { return m_iMaxAmmo; }

    //! Returns the rate of fire.
    int GetROF() const      { return m_iROF; }

    //! Returns the area of effect.
    int GetAOE() const      { return m_iAOE; }

    //! Serializes the descriptor.
    void Write( stringw &out ) const;
};