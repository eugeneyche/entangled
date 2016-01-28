#pragma once

#include <string>
#include <GL/glew.h>

struct GlyphMetrics
{
    int minx; 
    int miny;   
    int maxx; 
    int maxy;    
    int advance;
};

struct GlyphUVRect
{
    float minu;
    float maxu;
    float minv;
    float maxv;
};

class Font
{
public:
    constexpr static char FIRST_CHAR = ' ';
    constexpr static char LAST_CHAR = '~';
    constexpr static char NUM_GLYPHS = LAST_CHAR - FIRST_CHAR + 1;

    Font(const std::string& path, int size);
    ~Font();

    GLuint getAtlas() const;
    GlyphMetrics getGlyphMetrics(char c) const;
    GlyphUVRect getGlyphUVRect(char c) const;

private:
    GLuint atlas_ = 0u;
    GlyphMetrics glyph_metrics_ [NUM_GLYPHS];
    GlyphUVRect glyph_uv_rects_ [NUM_GLYPHS];
};
