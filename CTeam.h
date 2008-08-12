#pragma once
#include <list>
#include <string>
#include <SColor.h>
using namespace irr::video;

class CUnit;

class CTeam
{
private:
    typedef std::list<CUnit*> UnitArray;
    
    //! The units of this team.
    UnitArray m_units;

    //! The team's color.
    SColor m_color;

    //! The team's screen name.
    std::string m_sName;

public:
    //! Sets the team's color.
    void SetColor( const SColor &color ) { m_color = color; }
    
    //! Sets the screen name of the team.
    void SetName( const char *name )     { m_sName = name; }
    
    //! Adds a unit to the team.
    void Insert( CUnit *unit );

    //! Returns the team's color.
    const SColor& GetColor() const { return m_color; }
};