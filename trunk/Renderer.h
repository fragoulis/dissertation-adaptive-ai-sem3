#pragma once
#include <list>
#include "Singleton.h"

class IRenderable;

class Renderer : public Singleton<Renderer>
{
private:
    friend Singleton<Renderer>;
    typedef std::list<IRenderable*> Renderables;

    //! The array with pointers to renderable objects
    Renderables m_objects;

public:
    //! Renders all objects.
    void RenderAll();

    //! Adds a renderable interface to the array.
    void AddObject( IRenderable *obj ) { m_objects.push_front(obj); }

    //! Removes from the renderer.
    void RemoveObject( IRenderable *obj ) { m_objects.remove(obj); }
};