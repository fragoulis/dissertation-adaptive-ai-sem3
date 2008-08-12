#pragma once
#include "CUnit.h"
#include <fstream>
#include <vector>

/**
 * A game state consists of a represantation of the world at a given time.
 * Here, a game state holds the position of the actor and all the map
 * information that lie ahead of him for a certain depth.
 * The target of this is to be able to recognise each state in which the 
 * actor has been and pair it with an action and at the end with a fitness.
 * From every state the actor can move to one of four directions. The south
 * direction although available will probably never be chosen.
 *
 */
class CState
{
private:
    typedef std::vector<int> Features;

    //! The array of features that constitute a state.
    //! This invloves unit attributes an tile attributes.
    Features m_features;

public:
    //! Initializes the state.
    void Initialize( const CUnit &unit );

    //! Compares this state with another using manhattan distance.
    int ComputeDistance( const CState &other ) const;

    //! Compares this state with another using scaled manhattan distance.
    double ComputeDistanceScaled( const CState &other ) const;

    //! Returns the type id of the tile's occupier.
    int GetFeature( int index ) const { return m_features[index]; }

    //! Writes the state to a stream.
    void Save( std::ostream &out ) const;

    //! Loads the state from stream.
    void Load( std::istream &in );
};