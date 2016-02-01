#version 330 core

const int MAX_MESSAGE_LENGTH = 256;

layout(location = 0) in vec2 glyph_coord;

uniform mat4 viewport;

uniform vec4 atlas_bounds [MAX_MESSAGE_LENGTH];
uniform vec4 screen_bounds [MAX_MESSAGE_LENGTH];

smooth out vec2 f_tex_coord;

void main()
{
    f_tex_coord = vec2(
            atlas_bounds[gl_InstanceID][0] * (1 - glyph_coord.x) + 
            atlas_bounds[gl_InstanceID][1] * glyph_coord.x,
            atlas_bounds[gl_InstanceID][2] * (1 - glyph_coord.y) + 
            atlas_bounds[gl_InstanceID][3] * glyph_coord.y);
    vec2 screen_coord = vec2(
            screen_bounds[gl_InstanceID][0] * (1 - glyph_coord.x) + 
            screen_bounds[gl_InstanceID][1] * glyph_coord.x,
            screen_bounds[gl_InstanceID][2] * (1 - glyph_coord.y) + 
            screen_bounds[gl_InstanceID][3] * glyph_coord.y);

    gl_Position = viewport * vec4(screen_coord, 0, 1);
}
