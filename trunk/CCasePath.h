#pragma once
#include <list>
#include <iosfwd>

class CCase;

class CCasePath
{
private:
    typedef std::list<CCase*> Caselist;

    //! The list of cases followed by the unit from start to death.
    Caselist m_caselist;

    //! The fitness accompanying the case list.
    double m_fitness;

    //! Protection variable.
    //! Does not let the path to be reopened.
    bool m_closed;

public:
    //! Constructor.
    CCasePath();

    //! Adds a case.
    void AddCase( CCase *c ) { m_caselist.push_front(c); }

    //! Finalizes the path.
    void Close();

    //! Updates the path.
    void Update();

    //! Returns true if the size of the list is less than the 
    //! number of vertical tiles on the map.
    bool CheckSize() const;

    //! Returns the fitness.
    double GetFitness() const { return m_fitness; }

    //! Writes the path to a stream.
    void Save( std::ostream &out ) const;

private:
    //! Calculates the path's fitness.
    void _UpdateFitness();
};