#include "Map.h"
#include <cassert>

#include "./CColorManager.h"
#include "./CTeamManager.h"
#include "./CUnitManager.h"
#include "./CTerrainManager.h"
#include "./CAIManager.h"
#include "./Renderer.h"
#include "./Grid.h"
#include "./CObjectTypeManager.h"

// ----------------------------------------------------------------------------
Map::Map():
m_colorBkgr(255,220,220,220),
m_colorGrid(255,150,150,150),
m_bRenderGrid(true)
{
}

// ----------------------------------------------------------------------------
Map::~Map()
{
    Grid::Destroy();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Map::Load( const char *filename )
{
    m_filename = std::string(filename);
    IrrXMLReader *xml = createIrrXMLReader(filename);
    assert(xml);

    Grid& grid = Grid::Get();

    // read root tag first
    xml->read();
    const char *sName   = xml->getAttributeValue("name");
    grid.width   = xml->getAttributeValueAsInt("width");
    grid.height  = xml->getAttributeValueAsInt("height");
    int iTileX          = xml->getAttributeValueAsInt("tileX");
    int iTileY          = xml->getAttributeValueAsInt("tileY");
    const char *sColor  = xml->getAttributeValue("color");
    const char *sWire   = xml->getAttributeValue("wire");

    m_colorBkgr = *( MgrColor::Get().GetData(sColor) );
    m_colorGrid = *( MgrColor::Get().GetData(sWire) );
    grid.tilesize = dimension2di(iTileX, iTileY);

    assert(grid.width>0);
    assert(grid.height>0);

    grid.tilesX = grid.width / grid.tilesize.Width;
    grid.tilesY = grid.height / grid.tilesize.Height;
    grid.InitTiles();

    // Initialize tiles
    for( int row=0; row!=grid.tilesY; row++ )
        for( int col=0; col!=grid.tilesX; col++ )
            grid.SetTile(col, row);

    while(xml->read()) // move to read children colors
    {
        switch(xml->getNodeType())
        {
        case EXN_ELEMENT:
            {
                // read color attributes from file
                const char *sType = xml->getAttributeValue("type");
                const char *sTeam = xml->getAttributeValue("team");
                int iRow = xml->getAttributeValueAsInt("row");
                int iCol = xml->getAttributeValueAsInt("col");

                int iType = MgrObjectType::Get().GetTypeFromString(sType);

                if( sTeam ) // we have a unit
                {
                    // Allocate and initialize unit
                    CUnit *unit = MgrUnit::Get().CreateUnit(sType);
                    unit->SetInitialGridPos( position2di(iCol, iRow) );
                    unit->SetType(iType);
                    unit->Reset();

                    // Add it to its team
                    CTeam *team = MgrTeam::Get().GetData(sTeam);
                    team->Insert(unit);

                    // Push it to the renderer
                    Renderer::Get().AddObject(unit);
                }
                else { // we have another object

                    // Allocate and initialize terrain object
                    CTerrainObject *obj = MgrTerrain::Get().CreateObject(sType);
                    obj->Set(iCol, iRow);
                    obj->SetType(iType);
                    for(int row=0; row<obj->GetRows(); row++)
                    {
                        int y = obj->GetGridPosition().Y + row;
                        for( int col=0; col<obj->GetCols(); col++)
                        {
                            int x = obj->GetGridPosition().X + col;
                            grid.SetOccupier(x, y, obj);
                        }
                    }

                    // Push it to the renderer
                    Renderer::Get().AddObject(obj);
                }
            }
            break;
        }
    } // while 

    // delete the xml parser after usage
    delete xml;

    Renderer::Get().AddObject(this);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Map::Save()
{
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Map::Render() const
{
    if( !m_bRenderGrid ) return;

    Grid& grid = Grid::Get();
    int hTiles = (int)grid.width / grid.tilesize.Width;
    int vTiles = (int)grid.height / grid.tilesize.Height;

    position2d<s32> start(0,0), end(grid.width,0);

    for( int i=0; i<=vTiles; i++ )
    {
        start.Y = i * grid.tilesize.Height;
        end.Y = i * grid.tilesize.Height;

        DrawLine( start, end, m_colorGrid );
    }

    start.Y = 0;
    end.Y = grid.height;
    for( int i=0; i<=hTiles; i++ )
    {
        start.X = i * grid.tilesize.Width;
        end.X = i * grid.tilesize.Width;

        DrawLine( start, end, m_colorGrid );
    }
}