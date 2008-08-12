#pragma once
#include <vector>
#include <cassert>

#define CHECK_BOUNDS() assert( (index>=0) && (index<m_poolSize) )

template < class _Type >
class TPointerArray
{
private:
    typedef std::vector<_Type*> ObjArray;
    ObjArray m_objArray;
    _Type *m_pool;
    int m_poolSize;

public:
    TPointerVector(): m_pool(0) {}
    ~TPointerVector() { Destroy(); }

    void Create( int count ) 
    {
        assert(count>0);
        m_pool = new _Type[count];
        m_poolSize = count;
    }

    void Destroy()
    {
        m_objArray.clear();
        delete[] m_pool;
    }

    void Insert( int index ) { m_objArray.push_back(Get(index)); }
    _Type* Get( int index )  { CHECK_BOUNDS(); return &m_pool[index]; }
};