#pragma once
#include "TPointerMap.h"
#include "Singleton.h"
#include <irrString.h>
using irr::core::stringw;

#include <irrXML.h>
using namespace irr;
using namespace io;

template< class Type, class Single >
class IManager : public Singleton<Single>
{
protected:
    TPointerMap<stringw, Type> m_data;

public:
    virtual ~IManager() {}
    const Type* GetData( const stringw &key ) const { return m_data.Get(key); }
    Type* GetData( const stringw &key ) { return m_data.Get(key); }

    void Read( const char *filename )
    {
        IrrXMLReader *xml = createIrrXMLReader(filename);
        assert(xml);

        ReadXMLData(xml);

        delete xml;
    }

private:
    friend Singleton<Single>;
    virtual void ReadXMLData( IrrXMLReader *xml ) = 0;
}; 