#include "CGameEngine.h"
#include "./Irrlicht.h"
#include "Map.h"
#include "Renderer.h"
#include "Random.h"
#include <ctime>

#include "./CColorManager.h"
#include "./CTeamManager.h"
#include "./CUnitManager.h"
#include "./CTerrainManager.h"
#include "./CAIManager.h"
#include "./Grid.h"
#include "./CObjectTypeManager.h"
#include "./config.h"

// ----------------------------------------------------------------------------
CGameEngine::CGameEngine():
m_device(0),
m_pause(false),
m_step(true)
{
    Initialize();
}

// ----------------------------------------------------------------------------
CGameEngine::~CGameEngine()
{
    Deinitialize();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CGameEngine::Initialize()
{
    SRAND();

    config::init("data/config.xml");

    // Initialzie irrlicht device
    m_device = createDevice( 
        video::EDT_OPENGL,              // We will use the OpenGL device.
        config::wndSize,               // Window size. 
        16,                             // Amount of bits per pixel when in fullscreen mode.
        config::fullscreen,            // Fullscreen flag.
        false,                          // Stencil buffer use flag.
        false,                          // VSync on/off.
        this);                          // An EventReceiver object.

	m_device->setWindowCaption(L"Real-Time Strategy AI Testbed");

    // Acquire pointers to main interfaces to ease our access
    rts::video = m_device->getVideoDriver();
	rts::scene = m_device->getSceneManager();
	rts::gui   = m_device->getGUIEnvironment();

    ReadXMLs();

    // Initialize camera node
    ICameraSceneNode *cam = rts::scene->addCameraSceneNode(0, vector3df(0,100,0), vector3df(0,0,0));

    matrix4 projection;
    projection.buildProjectionMatrixOrthoLH(100.0f, 100.0f, -1.0f, 1.0f);
    cam->setProjectionMatrix(projection);

    Grid& grid = Grid::Get();
    int tx = ( grid.tilesX + 1 )* grid.tilesize.Width;
    int ty = grid.tilesize.Height;

    m_dbgText = rts::addStaticText(
        L"Press \"Space to start\"", 
        rect<s32>(tx,ty,300,300)
        );
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CGameEngine::ReadXMLs()
{
    // TODO: Read the filename's from file

    MgrColor::Get().Read("data/colors.xml" );
    MgrTeam::Get().Read("data/teams.xml" );
    MgrUnit::Get().Read("data/units.xml" );
    MgrTerrain::Get().Read("data/terrain.xml" );
    Map::Get().Load(config::active_map.c_str());
    MgrAI::Get().Read("data/ai.xml");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CGameEngine::Deinitialize()
{
    m_device->drop();

    Renderer::Destroy();
    MgrColor::Destroy();
    MgrTeam::Destroy();
    MgrAI::Destroy();
    MgrUnit::Destroy();
    MgrTerrain::Destroy();
    Map::Destroy();
    MgrObjectType::Destroy();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CGameEngine::Run()
{
    m_dtimer.Start();
    while(m_device->run())
	{
        float dt = m_dtimer.GetDeltaTime();
        WriteOutDebug(dt);
        if(!m_pause)
            Update( dt );

        rts::video->beginScene(true, true, Map::Get().GetBkgrColor());
		Render();
		rts::video->endScene();
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CGameEngine::Update(float dt)
{
    static int current_frame = MgrAI::Get().GetFrequency();

    if( current_frame == MgrAI::Get().GetFrequency() )
    {
        current_frame = 0;
        MgrAI::Get().Update();
        if(!m_pause) kNN::Get().Step();
        if(m_step) m_pause = true;
    }
    else
        current_frame++;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CGameEngine::WriteOutDebug(float dt)
{
    stringw out;
#ifdef _DEBUG
    out += L"FPS: ";
    out += rts::video->getFPS();
    out += L"\ndt: ";
    out += dt;
#endif
    out += L"\nPause is ";
    out += (m_pause?L"on":L"off");
    out += L"\nStep mode is ";
    out += (m_step?L"on":L"off");
    out += L"\nUpdate period: ";
    out += MgrAI::Get().GetFrequency();
    out += L"\n\n";
    MgrUnit::Get().GetActor().Write(out);
    kNN::Get().Write(out);
    out += L"\n";
    out += L"\"P\": to toggle pause mode.\n";
    out += L"\"O\": to toggle step mode. With step mode ON, press \"Space\" to move.\n";
    out += L"\"G\": to toggle grid.\n";
    out += L"\"F\": to toggle field of view.\n";
    out += L"\"D\": to toggle the previous position highlight.\n";

    m_dbgText->setText(out.c_str());
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CGameEngine::Render()
{
    Renderer::Get().RenderAll();
	rts::gui->drawAll();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool CGameEngine::OnEvent( const SEvent& event )
{
    if( event.EventType == EET_KEY_INPUT_EVENT &&
        !event.KeyInput.PressedDown )
    {
        switch( event.KeyInput.Key )
        {
        case KEY_KEY_S:
            CUnit::debug.drawId = !CUnit::debug.drawId;
            return true;

        case KEY_KEY_D:
            CUnit::debug.drawDir = !CUnit::debug.drawDir;
            return true;

        case KEY_KEY_F:
            CUnit::debug.drawFOV = !CUnit::debug.drawFOV;
            return true;

        case KEY_KEY_G:
            Map::Get().ToggleGridRender();
            return true;

        case KEY_KEY_P:
            m_pause = !m_pause;
            return true;

        case KEY_KEY_O:
            m_step = !m_step;
            m_pause = true;
            return true;

        case KEY_SPACE:
            m_pause = false;
            return true;

        case KEY_PLUS:
            MgrAI::Get().SpeedUp();
            return true;

        case KEY_MINUS:
            MgrAI::Get().SlowDown();
            return true;
        }
    }

    return false;
} 