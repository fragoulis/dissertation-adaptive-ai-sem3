#include "Renderer.h"
#include "IRenderable.h"

void Renderer::RenderAll()
{
    Renderables::iterator i = m_objects.begin();
    for(; i != m_objects.end(); ++i )
    {
        IRenderable *obj = *i;
        if(obj->IsActive()) obj->Render();
    }
}