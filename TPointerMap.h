#pragma once
#include <map>
#include <cassert>

#define CHECK_BOUNDS() assert( (index>=0) && (index<m_poolSize) )

template < class _Key, class _Type >
class TPointerMap
{
private:
    typedef std::map<_Key, _Type*> ObjMap;
    mutable ObjMap m_objMap;
    _Type *m_pool;
    int m_poolSize;

public:
    TPointerMap(): m_pool(0) {}
    ~TPointerMap() { Destroy(); }

    void Create( int count ) 
    {
        assert(count>0);
        m_pool = new _Type[count];
        m_poolSize = count;
    }

    void Destroy()
    {
        m_objMap.clear();
        delete[] m_pool;
    }

    
    void Insert( const _Key &key, int index )   { _Insert( key, Get(index) ); }
    void Set( const _Key &key, int index )      { _Set( key, Get(index) ); }
    _Type* Get( int index )                     { CHECK_BOUNDS(); return &m_pool[index]; }
    
    const _Type* Get( const _Key &key ) const   { return m_objMap[key]; }
    _Type* Get( const _Key &key )               { return m_objMap[key]; }

private:
    void _Insert( const _Key &key, _Type *value ) { m_objMap.insert( std::make_pair( key, value ) ); }
    void _Set( const _Key &key, _Type *value )    { m_objMap[key] = value; }
};

/**

TPointerMap<std::string, CColor*> map;

// read num of colors
map.Create(num_of_colors);

for all colors from file 
{
    map.GetFromPool( index, 
}

**/