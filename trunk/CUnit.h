#pragma once
#include "IObject.h"
#include "CUnitDesc.h"
#include "eAction.h"

class CUnitDesc;
class CTeam;

struct _debug 
{
    bool drawFOV;
    bool drawDir;
    bool drawId;
};

class CUnit : public IObject
{
public:
    //! A structure with debug flags.
    static _debug debug;

private:
    //! The initial position on the grid.
    position2di m_initialGridPos;

    //! The last position on the grid before the current.
    position2di m_previousGridPos;

    //! Unit's current health.
    int m_iHealth;

    //! Unit's current speed.
    int m_iSpeed;

    //! Pointer to the common unit attributes.
    CUnitDesc *m_desc;

    //! A pointer to the unit's team.
    CTeam *m_team;

    //! The direction is which the unit is moving.
    vector2df m_direction;

    //! The action to be followed.
    int m_action;

public:
    //! Constructor.
    CUnit();

    //! Sets an object's grid and real position
    virtual void Set( int x, int y );

    //! Sets the team.
    void SetTeam( CTeam *team ) { m_team = team; }

    //! Sets the initial position.
    void SetInitialGridPos( const position2di &pos ) { m_initialGridPos = pos; }

    //! Sets the unit's descriptor.
    void SetDesc( CUnitDesc *desc );

    //! Sets the unit's direction.
    void SetDirection( const vector2df &dir ) { m_direction = dir; }

    //! Returns the health.
    int GetHealth() const { return m_iHealth; }

    //! Returns the speed.
    int GetSpeed() const { return m_iSpeed; }

    //! Returns the field of view.
    int GetFOV() const { return m_desc->GetFOV(); }

    //! Returns the maximum speed.
    int GetMaxSpeed() const { return m_desc->GetMaxSpeed(); }

    //! Returns the speed manipulation ratio.
    int GetSpeedRatio() const { return m_desc->GetSpeedRatio(); }

    //! Returns the maximum health.
    int GetMaxHealth() const { return m_desc->GetMaxHealth(); }

    //! Get attack.
    int GetAttack() const { return m_desc->GetAttack(); }

    //! Get defence.
    int GetDefence() const { return m_desc->GetDefence(); }

    //! Returns the unit's descriptor.
    CUnitDesc* GetDesc() { return m_desc; }

    //! Increases the unit's speed by the ratio.
    void IncreaseSpeed() { m_iSpeed += m_desc->GetSpeedRatio(); }

    //! Decreases the unit's speed by the ratio.
    void DecreaseSpeed() { m_iSpeed -= m_desc->GetSpeedRatio(); }

    //! Nulls the speed.
    void Stop() { m_iSpeed = 0; }

    //! Moves the unit according and returns a fitness.
    double ActOn( E_ACTION action );

    //! Resets the unit.
    void Reset();

    //! Copy's the attributes of another object.
    virtual void CloneFrom( IObject *obj );

    //! Renders the object.
    virtual void Render() const;

    //! Updates the object
    virtual void Update(float dt);

    //! Serializes the object.
    virtual void Write( stringw &out ) const;

    //! This is executed after rendering.
    void Update();
};