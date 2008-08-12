#include "CObjectTypeManager.h"
#include "CHash.h"

// ----------------------------------------------------------------------------
int CObjectTypeManager::AddType( const stringw& key ) 
{ 
    int value = (int)CHash::_(key.c_str());
    m_types.insert( std::make_pair(key, value) );

    return value;
}

// ----------------------------------------------------------------------------
const stringw& CObjectTypeManager::GetTypeFromInt( const int type ) const
{
    ObjectTypes::const_iterator it = m_types.begin();
    for(; it != m_types.end(); ++it )
    {
        if( type == it->second ) return it->first;
    }

    assert(0);
    return it->first;
}

// ----------------------------------------------------------------------------
bool CObjectTypeManager::IsOfType( const stringw& key, const int type ) const
{
    return ( (int)CHash::_(key.c_str()) == type );
}