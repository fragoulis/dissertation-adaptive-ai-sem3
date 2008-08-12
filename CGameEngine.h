#pragma once
#include "./Irrext.h"
#include "./DeltaTimer.h"

class CGameEngine : public IEventReceiver 
{
public:
    //! Constructor
    CGameEngine();

    //! Destructor 
    ~CGameEngine();

    //! Runs the main loop
    void Run();

    //! Initializes the engine and all necessary interfaces
    void Initialize();

    //! Deinitializes the engine and drops interfaces
    void Deinitialize();

    //! Updates the game
    void Update(float dt);

    //! Renders the scene
    void Render();

    //! Reads all data from xml files
    void ReadXMLs();

    //! Receives system events.
    virtual bool OnEvent( const SEvent& event );

    //! Writes out debug information to window.
    void WriteOutDebug(float dt);

private: // variables

    //! Irrlicht's main device object
    IrrlichtDevice *m_device;

    //! Debugging text.
    IGUIStaticText *m_dbgText;

    //! The delta time timer.
    DeltaTimer m_dtimer;

    //! Pause flag.
    bool m_pause;
    
    //! Step flag.
    bool m_step;
};