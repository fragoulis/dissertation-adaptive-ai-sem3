#pragma once
#include "CAction.h"
#include "CState.h"

class CCase : public IRenderable
{
private:
    //! The state.
    CState m_state;

    //! The action.
    CAction m_action;

    //! The fitness resulting from the action taken at the specific state.
    double m_fitness;

public:
    //! Constructor.
    CCase();

    //! Initializes the case.
    void Initialize( const CUnit &unit );

    //! Returns a const pointer to the associated state.
    const CState& GetState() const { return m_state; }

    //! Returns a non-const pointer to the associated state.
    CState& GetState() { return m_state; }

    //! Returns the fitness.
    double GetFitness() const { return m_fitness; }

    //! Compares this case with another using manhattan distance.
    int ComputeDistance( const CCase &other ) const;

    //! Returns a const pointer to the action.
    E_ACTION GetAction() const { return m_action.GetDirection(); }

    //! Sets the action.
    void SetAction( E_ACTION action ) { m_action.SetDirection(action); }

    //! Sets the fitness.
    void SetFitness( double fitness ) { m_fitness = fitness; }

    //! Writes out the case data to file.
    void Save( std::ostream &out ) const;

    //! Loads the case from stream.
    void Load( std::istream &in );

    //! Represents the case graphically.
    virtual void Render() const;
};