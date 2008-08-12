#pragma once
#include <SColor.h>
#include <irrString.h>
using irr::core::stringw;
using irr::core::stringc;
using irr::video::SColor;

class CUnitDesc
{
private:
    //! The printable name of the unit.
    stringw m_sName;

    //! The gun type.
    stringc m_sGunType;

    //! The unit's field of view.
    int m_iFOV;

    //! The maximum speed.
    int m_iMaxSpeed;

    //! Speed manipulation ratio.
    int m_iSpeedRatio;

    //! The maximum health.
    int m_iMaxHealth;

    //! The chance of successfull attack.
    int m_iAttack;

    //! The chance of defending.
    int m_iDefence;

    //! The specific color.
    SColor m_color;

public:
    //! Sets the name.
    void SetName( const char *name )    { m_sName = name; }

    //! Sets the gun type.
    void SetGunType( const char *gun )  { m_sGunType = gun; }

    //! Sets the field of view.
    void SetFOV( int fov )              { m_iFOV = fov; }

    //! Sets the maximum speed.
    void SetMaxSpeed( int speed )       { m_iMaxSpeed = speed; }

    //! Sets the speed manipulation ratio.
    void SetSpeedRatio( int ratio )     { m_iSpeedRatio = ratio; }

    //! Sets the maximum health.
    void SetMaxHealth( int health )     { m_iMaxHealth = health; }

    //! Sets the attack value.
    void SetAttack( int attack )        { m_iAttack = attack; }

    //! Sets the defence value.
    void SetDefence( int defence )      { m_iDefence = defence; }

    //! Sets the color.
    void SetColor( const SColor &color ) { m_color = color; }

    //! Returns the gun type.
    const char* GetGunType() const  { return m_sGunType.c_str(); }

    //! Returns the field of view.
    int GetFOV() const              { return m_iFOV; }

    //! Returns the maximum speed.
    int GetMaxSpeed() const         { return m_iMaxSpeed; }

    //! Returns the speed manipulation ratio.
    int GetSpeedRatio() const       { return m_iSpeedRatio; }

    //! Returns the maximum health.
    int GetMaxHealth() const        { return m_iMaxHealth; }

    //! Get attack.
    int GetAttack() const           { return m_iAttack; }

    //! Get defence.
    int GetDefence() const          { return m_iDefence; }

    //! Return the color.
    const SColor& GetColor() const { return m_color; }
    
    //! Serializes the structure.
    void Write( stringw &out ) const;
};