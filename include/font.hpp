#pragma once

#include <string>
#include <GL/glew.h>

struct GlyphMetrics
{
    int min_x; 
    int max_x;   
    int min_y; 
    int max_y;    
    int advance;
};

struct GlyphBound
{
    float min_s;
    float min_t;
    float max_s;
    float max_t;
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
    GlyphBound getGlyphBound(char c) const;

private:
    GLuint atlas_ = 0u;
    GlyphMetrics glyph_metrics_map_ [NUM_GLYPHS];
    GlyphBound glyph_bound_map_ [NUM_GLYPHS];
};
