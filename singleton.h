#pragma once
#include <cassert>

template<class T>
class Singleton
{
public:
    //! Returns the object but also checks if the object exists
    static T& Get()
    {
        if( 0 == pInstance ) {  
            pInstance = new T;
        }

        return *pInstance;
    }

    //! Destroys the object
    static void Destroy()
    {
        delete pInstance;
        pInstance = 0;
    }
    
protected:
    // Declare constructor, copy constructor,
    // assignment operator and destructor protected
    Singleton() {}
    Singleton(const Singleton&);
    Singleton& operator= (const Singleton&);
    virtual ~Singleton() = 0 {}

    //! The object's unique instance
    static T* pInstance;

}; // end Singleton

template<class T>
T* Singleton<T>::pInstance = 0;