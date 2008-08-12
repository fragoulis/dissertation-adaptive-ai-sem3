#include "CTile.h"
#include "stlastar/stlastar.h"
#include "Grid.h"

#define _OUT(label, value) out += label; out += value; out += "\n";
//#define _USE_DIAGONAL_PATH

// ----------------------------------------------------------------------------
CTile::CTile():
m_color(SColor(250, 250, 0, 0)),
m_occupier(0)
{
}

// ----------------------------------------------------------------------------
CTile::CTile( int x, int y ):
m_color(SColor(250, 250, 0, 0)),
m_occupier(0)
{
    Set(x, y);
}

// ----------------------------------------------------------------------------
void CTile::Render() const
{
    DrawRectangle( m_color, m_realPosition );
}

// ----------------------------------------------------------------------------
float CTile::GoalDistanceEstimate( CTile &nodeGoal )
{
    int xd = abs(m_gridPosition.X - nodeGoal.GetGridPosition().X);
	int yd = abs(m_gridPosition.Y - nodeGoal.GetGridPosition().Y);

	return float(xd + yd);
}

// ----------------------------------------------------------------------------
bool CTile::IsGoal( CTile &nodeGoal )
{
    if( m_gridPosition.X == nodeGoal.GetGridPosition().X &&
        m_gridPosition.Y == nodeGoal.GetGridPosition().Y )
    {
        return true;
    }

	return false;
}

// ----------------------------------------------------------------------------
bool CTile::IsGoal( int x, int y )
{
    CTile& goal = Grid::Get().GetTile(x, y);
    return IsGoal(goal);
}

// ----------------------------------------------------------------------------
bool CTile::IsSameState( CTile &rhs )
{
	// same state in a maze search is simply when (x,y) are the same
	if( (m_gridPosition.X == rhs.GetGridPosition().X) &&
		(m_gridPosition.Y == rhs.GetGridPosition().Y) )
	{
		return true;
	}
	
    return false;
}

// ----------------------------------------------------------------------------
// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool CTile::GetSuccessors( AStarSearch<CTile> *astarsearch, CTile *parent_node )
{

	int parent_x = -1; 
	int parent_y = -1; 

	if( parent_node )
	{
		parent_x = parent_node->GetGridPosition().X;
		parent_y = parent_node->GetGridPosition().Y;
	}
	
    int x = m_gridPosition.X;
    int y = m_gridPosition.Y;

	// push each possible move except allowing the search to go backwards

    // -------------------------------------------------
	if( x > 0 &&
        Grid::Get().IsFree( x-1, y ) &&
		!((parent_x == x-1) && (parent_y == y))
        ) 
	{
        astarsearch->AddSuccessor( Grid::Get().GetTile(x-1, y) );
    }

    // -------------------------------------------------
	if( y > 0 &&
        Grid::Get().IsFree( x, y-1 ) &&
		!((parent_x == x) && (parent_y == y-1))
        )
	{
		astarsearch->AddSuccessor( Grid::Get().GetTile(x, y-1) );
	} 

    // -------------------------------------------------
    if( x < Grid::Get().tilesX - 1 &&
        Grid::Get().IsFree( x+1, y ) &&
		!((parent_x == x+1) && (parent_y == y))
        ) 
	{
		astarsearch->AddSuccessor( Grid::Get().GetTile(x+1, y) );
	} 

	// -------------------------------------------------
	if( y < Grid::Get().tilesY - 1 &&
        Grid::Get().IsFree( x, y+1 ) &&
		!((parent_x == x) && (parent_y == y+1))
		)
	{
		astarsearch->AddSuccessor( Grid::Get().GetTile(x, y+1) );
    }

#ifdef _USE_DIAGONAL_PATH
    // -------------------------------------------------
    if( x > 0 &&
        y > 0 &&
        Grid::Get().IsFree( x-1, y-1 ) &&
		!((parent_x == x-1) && (parent_y == y-1))
        ) 
	{
		astarsearch->AddSuccessor( Grid::Get().GetTile(x-1, y-1) );
	}

	// -------------------------------------------------
	if( x < Grid::Get().tilesX - 1 && 
        y < Grid::Get().tilesY - 1 &&
        Grid::Get().IsFree( x+1, y+1 ) &&
		!((parent_x == x+1) && (parent_y == y+1))
        )
	{
		astarsearch->AddSuccessor( Grid::Get().GetTile(x+1, y+1) );
	}

    // -------------------------------------------------
	if( x > 0 && 
        y < Grid::Get().tilesY - 1 &&
        Grid::Get().IsFree( x-1, y+1 ) &&
		!((parent_x == x-1) && (parent_y == y+1))
        )
	{
		astarsearch->AddSuccessor( Grid::Get().GetTile(x-1, y+1) );
	}

    // -------------------------------------------------
	if( x < Grid::Get().tilesX - 1 && 
        y > 0 &&
        Grid::Get().IsFree( x+1, y-1 ) &&
		!((parent_x == x+1) && (parent_y == y-1))
        )
	{
		astarsearch->AddSuccessor( Grid::Get().GetTile(x+1, y-1) );
	}
#endif // _USE_DIAGONAL_PATH

	return true;
}

// ----------------------------------------------------------------------------
float CTile::GetCost( CTile &successor )
{
	return 1;
}