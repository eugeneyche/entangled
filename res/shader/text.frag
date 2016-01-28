#version 330 core

smooth in vec2 f_tex_coord;

uniform vec3 color;
uniform sampler2D atlas;

out vec4 output_color;

void main()
{
    output_color = vec4(color, 1) * texture(atlas, f_tex_coord);
}
