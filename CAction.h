#pragma once
#include "eAction.h"

class CAction
{
private:
    //! Action's direction.
    E_ACTION m_direction;

public:
    //! Constructor.
    CAction();

    //! Sets the direction.
    void SetDirection( E_ACTION action ) { m_direction = action; }

    //! Returns the action direction.
    E_ACTION GetDirection() const { return m_direction; }
};