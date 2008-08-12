#pragma once
#include "Singleton.h"
#include "kNN.h"
#include <vector>

class CAIManager : public Singleton<CAIManager>
{
private:
    //! The frequency of the update.
    int m_iFrequency;

public:
    //! Updates all unti AIs.
    void Update();

    //! Sets the update frequency.
    void SetFrequency( int f ) { m_iFrequency = f; }

    //! Returns the update frequency.
    int GetFrequency() const { return m_iFrequency; }

    //! Increases the frequency by 50%.
    void SpeedUp();

    //! Decreases the frequency by 50%.
    void SlowDown();

    //! Initializes the structure from xml file.
    void Read( const char *filename );

private:
    friend Singleton<CAIManager>;

    //! Constructor.
    CAIManager();

    //! Destructor.
    ~CAIManager();
};

typedef CAIManager MgrAI;