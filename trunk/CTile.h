#pragma once
#include "IObject.h"

template <class T> class AStarSearch;

class CTile : public IObject
{
private:
    //! Pointer to the object occupying the tile.
    IObject *m_occupier;

    //! General color flag.
    SColor m_color;

public:
    //! Constructor.
    CTile();

    //! Constructor.
    CTile( int x, int y );

    //! Return the color.
    const SColor& GetColor() const { return m_color; }

    //! Sets the occupier.
    void SetOccupier( IObject *occ ) { m_occupier = occ; }

    //! Returns a const pointer to the occupier.
    const IObject* GetOccupier() const { return m_occupier; }

    //! Returns a non-const pointer to the occupier.
    IObject* GetOccupier() { return m_occupier; }

    //! Sets the color.
    void SetColor( const SColor &color ) { m_color = color; }

    //! Copy's the attributes of another object.
    virtual void CloneFrom( IObject *obj ) {}

    //! Renders the object.
    virtual void Render() const;

    //! Updates the object
    virtual void Update(float dt) {}

    //! Serializes the object.
    virtual void Write( stringw &out ) const {}

    // -- Pathfinding specific funtions

    float GoalDistanceEstimate( CTile &nodeGoal );
	bool IsGoal( CTile &nodeGoal );
    bool IsGoal( int x, int y );
	bool GetSuccessors( AStarSearch<CTile> *astarsearch, CTile *parent_node );
	float GetCost( CTile &successor );
	bool IsSameState( CTile &rhs );

};
