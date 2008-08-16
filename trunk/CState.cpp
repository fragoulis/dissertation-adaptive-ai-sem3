#include "CState.h"
#include "Grid.h"
#include "logger.h"
#include "CColorManager.h"
#include "CObjectTypeManager.h"
#include "CTerrainManager.h"

const int FEATURES = 60;

// ----------------------------------------------------------------------------
scaled_values CState::s_scaled;

// ----------------------------------------------------------------------------
void CState::Render() const
{
    Grid& grid = Grid::Get();

    const SColor &grey = * (MgrColor::Get().GetData("light_grey"));
    
    const int endX = m_position.X + s_scaled.gridWidth - s_scaled.tileWidth;
    const int nLinesHoriz = s_scaled.fov + 1;
    for( int line=0; line<nLinesHoriz; line++ )
    {
        const int lineY = m_position.Y + line * s_scaled.tileHeight;
        DrawLine(
            position2di(m_position.X, lineY),
            position2di(endX, lineY),
            grey
            );
    }

    const int endY = m_position.Y + s_scaled.gridHeight;
    const int nLinesVert = grid.tilesX - 1;
    for( int line=1; line<=nLinesVert; line++ )
    {
        const int lineX = m_position.X + line * s_scaled.tileWidth;
        DrawLine(
            position2di(lineX, m_position.Y),
            position2di(lineX, endY),
            grey
            );
    }

    Features::const_iterator it = m_features.begin();
    for(
        int i=0; 
        it != m_features.end(); 
        ++it, ++i )
    {
        int iType = *it;
        if( iType==0 ) continue;

        const stringw &sType = MgrObjectType::Get().GetTypeFromInt(iType);
        const CTerrainObject *obj = MgrTerrain::Get().GetData(sType);

        const int x = (m_x + i) % grid.tilesX;
        const int y = ( i / grid.tilesX );
        DrawRectangle(
            obj->GetColor(), 
            position2di(
                m_position.X + x*s_scaled.tileWidth, 
                m_position.Y + y*s_scaled.tileHeight
                ),
            dimension2di(
                s_scaled.tileWidth,
                s_scaled.tileHeight
                )
            );
    }

    DrawRectangle(
        * (MgrColor::Get().GetData("green")), 
        position2di(
            m_position.X + m_x * s_scaled.tileWidth, 
            m_position.Y - s_scaled.tileHeight
            ),
        dimension2di(
            s_scaled.tileWidth,
            s_scaled.tileHeight
            )
        );
}

// ----------------------------------------------------------------------------
int CState::GetFeature( int index ) const 
{ 
    if( index < 0 )
        index = (int)m_features.size() + index;

    return m_features[index]; 
}

// ----------------------------------------------------------------------------
void CState::Initialize( const CUnit &unit )
{
    // use field of view to determine how many tiles the unit sees ahead
    const int fov = unit.GetFOV();

    Grid& grid = Grid::Get();
    const int px = unit.GetGridPosition().X;
    const int py = unit.GetGridPosition().Y;

    assert(px>=0 && px<grid.tilesX);
    assert(py>=0 && py<grid.tilesY);

    const int start   = 0;
    const int end     = grid.tilesX * fov;
    const int abs_end = grid.tilesX * grid.tilesY;
    
    for( int i=start; i<end; i++ )
    {
        const int x = (px + i) % grid.tilesX;
        const int y = ( i / grid.tilesX ) + 1;

        const int index = x + (py + y) * grid.tilesX;

        int type = 0;
        if( index < abs_end )
        {
            const CTile& tile = grid.GetTile(index);
            if( tile.GetOccupier() ) type = tile.GetOccupier()->GetType();
        }

        m_features.push_back(type);
    }

    m_x = px;
}

// ----------------------------------------------------------------------------
void CState::InitializeOLD( const CUnit &unit )
{
    // use field of view to determine how many tiles the unit sees ahead
    const int fov = unit.GetFOV();

    Grid& grid = Grid::Get();
    int x = unit.GetGridPosition().X;
    int y = unit.GetGridPosition().Y;

    assert(x>=0 && x<grid.tilesX);
    assert(y>=0 && y<grid.tilesY);

    // read all indices from (x,y) where x equals the first tile at the 
    // row is on and y equals its row until row equals the fov

    int start   = ( y + 1 ) * grid.tilesX;
    int end     = grid.tilesX + ( y + fov ) * grid.tilesX;
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
    for(; it != m_features.end(); ++it )
    {
        out << *it << " ";
    }

    out << m_x;
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

    in >> m_x;
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