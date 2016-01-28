#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 world_view_projection;

void main()
{
    gl_Position = world_view_projection * vec4(position, 1);
}
