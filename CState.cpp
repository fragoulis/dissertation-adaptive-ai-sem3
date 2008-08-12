#include "CState.h"
#include "Grid.h"
#include "logger.h"

#define _USE_POSITION_FEATURE_

#ifdef _USE_POSITION_FEATURE_
const int FEATURES = 73;
#else
const int FEATURES = 72;
#endif

// ----------------------------------------------------------------------------
void CState::Initialize( const CUnit &unit )
{
    // use field of view to determine how many tiles the unit sees ahead
    int fov = unit.GetFOV();

    Grid& grid = Grid::Get();
    int x = unit.GetGridPosition().X;
    int y = unit.GetGridPosition().Y;

    assert(x>=0 && x<grid.tilesX);
    assert(y>=0 && y<grid.tilesY);

    // read all indices from (x,y) where x equals the first tile at the 
    // row is on and y equals its row until row equals the fov

    int start   = ( y + 1 ) * grid.tilesX;
    int end     = grid.tilesX + ( y + 1 + fov ) * grid.tilesX;
    int to      = end;
    int abs_end = grid.tilesX * grid.tilesY;
    int weight  = 1;

    if( end >= abs_end ) to = abs_end;
    
    for( int i=start; i<to; i++ )
    {
        const CTile& tile = grid.GetTile(i);

        int type = 0;
        if( tile.GetOccupier() ) type = tile.GetOccupier()->GetType();
        m_features.push_back(type * (weight++));
    }

    if( end >= abs_end )
    {
        end -= abs_end;
        for( int i=0; i<end; i++ ) m_features.push_back(0);
    }

#ifdef _USE_POSITION_FEATURE_
    m_features.push_back(unit.GetGridPosition().X);
    //m_features.push_back(unit.GetGridPosition().Y);
    //m_features.push_back(unit.GetHealth());
#endif
}

// ----------------------------------------------------------------------------
void CState::Save( std::ostream &out ) const
{
    Features::const_iterator it = m_features.begin();
    for(
        int i=0; 
        it != m_features.end(); 
        ++it, ++i )
    {
        out << *it;
        if( i < FEATURES-1 )
            out << " ";
    }
}

// ----------------------------------------------------------------------------
void CState::Load( std::istream &in )
{
    m_features.clear();
    for( int i=0; i<FEATURES; i++ )
    {
        int feature;
        in >> feature;
        m_features.push_back(feature);
    }
}

// ----------------------------------------------------------------------------
int CState::ComputeDistance( const CState &other ) const
{
    int N = (int)m_features.size();    // number of attributes
   
    // distance between x and y attributes
    int dXY = 0;

    // find the sum of the absolute difference of the features of the two states
    for( int i=0; i != N; i++ )
    {
        dXY += abs(m_features[i] - other.GetFeature(i));
    }
    //_log_n("Distance : ", dXY);

    return dXY;
}

// ----------------------------------------------------------------------------
double CState::ComputeDistanceScaled( const CState &other ) const
{
    int N = (int)m_features.size();    // number of attributes
    
    // ________________________________________________________________________
    // sum of x attributes
    int sX  = 0;
    
    // find the arithmetic mean of the features of the state.
    for( int i=0; i != N; i++ ) {
        sX += m_features[i];
    }

    // arithmetic mean of x attributes
    double amX = sX / N;

    // get the deviation
    double samX = 0;
    for( int i=0; i != N; i++ ) {
        double diff = m_features[i] - amX;
        samX += diff*diff;
    }
    double deviationX = ::sqrt( (double)samX / N );
    assert(deviationX!=0.0);

    // ________________________________________________________________________
    // sum of y attributes
    int sY  = 0;
    
    // find the arithmetic mean of the features of the state.
    for( int i=0; i != N; i++ ) {
        sY += other.GetFeature(i);
    }

    // arithmetic mean of x attributes
    double amY = sY / N;

    // get the deviation
    double samY = 0;
    for( int i=0; i != N; i++ ) {
        double diff = other.GetFeature(i) - amY;
        samY += diff*diff;
    }
    double deviationY = ::sqrt( (double)samY / N );
    assert(deviationY!=0.0);

    // ________________________________________________________________________
    // distance between scaled x and y attributes
    double dXY = 0;

    // find the sum of the absolute difference of the features of the two states.
    for( int i=0; i != N; i++ )
    {
        double scaledX = ( m_features[i] - amX ) / deviationX;
        double scaledY = (other.GetFeature(i) - amY ) / deviationY;
        
        dXY += abs(scaledX - scaledY);
    }
    _log_n("Distance : ", dXY);

    return dXY;
}