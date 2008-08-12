#pragma once
#include <iosfwd>
using std::ostream;

class CColor
{
private:
    float m_rgb[3];

public:
    CColor() { Set(0.0f, 0.0f, 0.0f); }
    CColor( float r, float g, float b ) { Set(r, g, b); }
    
    void Set( float r, float g, float b ) 
    { 
        m_rgb[0] = r;
        m_rgb[1] = g;
        m_rgb[2] = b;
    }

    const float* Get() const { return m_rgb; }

    float GetRed() const { return m_rgb[0]; }
    float GetGreen() const { return m_rgb[1]; }
    float GetBlue() const { return m_rgb[2]; }

    float SetRed( float r ) { m_rgb[0] = r; }
    float SetGreen( float g ) { m_rgb[1] = g; }
    float SetBlue( float b ) { m_rgb[2] = b; }

    void Write( ostream &out ) const;
};

//ostream& operator << ( ostream &out, const CColor &color )
//{
//    color.Write(out);
//    return out;
//}