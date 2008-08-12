#pragma once
#include <string>
#include "vector2d.h"
#include <SColor.h>
#include "IRenderable.h"
#include "Singleton.h"

using namespace irr;
using video::SColor;

class Map : public IRenderable, public Singleton<Map>
{
private:
    friend Singleton<Map>;

    //! The name of map to load.
    std::string m_filename;

    //! The map's background color.
    SColor m_colorBkgr;

    //! The map's grid color.
    SColor m_colorGrid;

    //! Flag for wether to render the grid or not.
    bool m_bRenderGrid;

    //! Constructor.
    Map();

    //! Destructor.
    ~Map();

public:
    //! Returns the name of the map.
    const std::string& GetName() const { return m_filename; }

    //! Reads the map from file.
    void Load( const char *filename );

    //! Saves the loaded map to file.
    void Save();

    //! Returns the background color.
    const SColor& GetBkgrColor() const { return m_colorBkgr; }

    //! Returns the grid color.
    const SColor& GetGridColor() const { return m_colorGrid; }

    //! Toggles the grid render.
    void ToggleGridRender() { m_bRenderGrid = !m_bRenderGrid; }

    //! Renders the map.
    virtual void Render() const;
};