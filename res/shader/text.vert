#version 330 core

const int MAX_MESSAGE_LENGTH = 256;

layout(location = 0) in vec2 glyph_coord;

uniform float screen_width;
uniform float screen_height;

uniform vec4 uv_rects [MAX_MESSAGE_LENGTH];
uniform vec4 screen_rects [MAX_MESSAGE_LENGTH];

smooth out vec2 f_tex_coord;

void main()
{
    f_tex_coord = vec2(
            uv_rects[gl_InstanceID][0] * (1 - glyph_coord.x) + 
            uv_rects[gl_InstanceID][1] * glyph_coord.x,
            uv_rects[gl_InstanceID][2] * (1 - glyph_coord.y) + 
            uv_rects[gl_InstanceID][3] * glyph_coord.y);
    vec2 screen_coord = vec2(
            screen_rects[gl_InstanceID][0] * (1 - glyph_coord.x) + 
            screen_rects[gl_InstanceID][1] * glyph_coord.x,
            screen_rects[gl_InstanceID][2] * (1 - glyph_coord.y) + 
            screen_rects[gl_InstanceID][3] * glyph_coord.y);

    gl_Position = vec4(
            -1 + 2 * screen_coord.x / screen_width,
            -1 + 2 * screen_coord.y / screen_height,
            0, 1);
}
