#pragma once
#include <irrString.h>
using irr::core::stringw;

/**
 * @class CHash
 * 
 * Stores an string in integer format
 */
class CHash
{
public:
    //! Returns the hash for the given string.
    //! The first time this operator is called, it caches the result and
    //! from then on returns the hashed value directly
    static unsigned _( const wchar_t *str )
    {
        unsigned hash = 0;
        unsigned len = (unsigned)wcslen(str);
        unsigned i=0;
        for( ; i<len; ++i )
            hash += (i+1) * str[i];

        return hash;
    }

}; // end CHash