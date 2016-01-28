#include <iostream>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include "error.hpp"
#include "font.hpp"

Font::Font(const std::string& path, int size)
{
    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if (!font)
    {
        fatalError("Failed to open font '" + path + "'.");
    }
    SDL_Color color {255, 255, 255, 255};
    SDL_Surface* surfaces [NUM_GLYPHS];
    for (int i = 0; i < NUM_GLYPHS; i++)
    {
        char c = FIRST_CHAR + i;
        TTF_GlyphMetrics(
                font, c, 
                &glyph_metrics_[i].minx,
                &glyph_metrics_[i].maxx,
                &glyph_metrics_[i].miny,
                &glyph_metrics_[i].maxy,
                &glyph_metrics_[i].advance);
        surfaces[i] = TTF_RenderGlyph_Blended(font, c, color);
    }
    int row_height = TTF_FontHeight(font);
    int row_offset = -TTF_FontDescent(font) + 1;
    int num_glyphs_per_row = 16;
    int num_rows = (NUM_GLYPHS - 1) / num_glyphs_per_row + 1;
    int max_row_width = 0;
    for (int i = 0; i < num_rows; i++)
    {
        int row_width = 0;
        for (int j = 0; j < num_glyphs_per_row; j++)
        {
            int index = i * num_glyphs_per_row + j;
            if (index >= NUM_GLYPHS)
                break;
            row_width += surfaces[index]->w;
        }
        if (row_width > max_row_width)
            max_row_width = row_width;
    }
    int atlas_height = num_rows * row_height;
    int atlas_width =  max_row_width;
    int cursor_x = 0;
    int cursor_y = 0;
    int bytes_per_pixel = 4;
    std::vector<uint8_t> atlas_pixels (bytes_per_pixel * atlas_height * atlas_width);
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_glyphs_per_row; j++)
        {
            int index = i * num_glyphs_per_row + j;
            if (index >= NUM_GLYPHS)
                break;
            SDL_Surface* surface = surfaces[index];
            for (int k = 0; k < row_height; k++)
            {
                for (int l = 0; l < surface->w; l++)
                {
                    int atlas_index = bytes_per_pixel * ((cursor_y + k) * atlas_width + cursor_x + l);
                    int surface_index = bytes_per_pixel * ((surface->h - k - 1) * surface->w + l);
                    for (int m = 0; m < bytes_per_pixel; m++)
                    {
                        atlas_pixels[atlas_index + m] = reinterpret_cast<uint8_t*>(surface->pixels)[surface_index + m];
                    }
                }
            }
            GlyphMetrics metrics = glyph_metrics_[index];
            glyph_uv_rects_[index].minu = float(cursor_x + metrics.minx) / atlas_width;
            glyph_uv_rects_[index].maxu = float(cursor_x + metrics.maxx) / atlas_width;
            glyph_uv_rects_[index].minv = float(cursor_y + row_offset + metrics.miny) / atlas_height;
            glyph_uv_rects_[index].maxv = float(cursor_y + row_offset + metrics.maxy) / atlas_height;
            cursor_x += surface->w;
        }
        cursor_x = 0;
        cursor_y += row_height;
    }
    for (int i = 0; i < NUM_GLYPHS; i++)
    {
        SDL_FreeSurface(surfaces[i]);
    }
    TTF_CloseFont(font);
    glGenTextures(1, &atlas_);
    glBindTexture(GL_TEXTURE_2D, atlas_);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            atlas_width,
            atlas_height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            atlas_pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

Font::~Font()
{
    glDeleteTextures(1, &atlas_);
}

GLuint Font::getAtlas() const
{
    return atlas_;
}

GlyphMetrics Font::getGlyphMetrics(char c) const
{
    return glyph_metrics_[c - FIRST_CHAR];
}

GlyphUVRect Font::getGlyphUVRect(char c) const
{
    return glyph_uv_rects_[c - FIRST_CHAR];
}

