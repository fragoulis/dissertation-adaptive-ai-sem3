#pragma once
#include "Singleton.h"
#include <map>
#include <irrString.h>
using irr::core::stringw;

class CObjectTypeManager : public Singleton<CObjectTypeManager>
{
private:
    typedef std::map< stringw, int > ObjectTypes;

    //! The object types.
    mutable ObjectTypes m_types;

public:
    //! Adds a type.
    int AddType( const stringw& key );

    //! Returns a type.
    int GetTypeFromString( const stringw& key ) const { return m_types[key]; }

    //! Returns the key for a value.
    const stringw& GetTypeFromInt( const int type ) const;

    //! Compares a string with a integer type.
    bool IsOfType( const stringw& key, const int type ) const;
};

typedef CObjectTypeManager MgrObjectType;