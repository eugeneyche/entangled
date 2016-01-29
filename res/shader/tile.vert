#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 world_view;
uniform mat4 it_world_view;
uniform mat4 world_view_projection;

smooth out vec3 frag_position;
smooth out vec3 frag_normal;

void main()
{
    frag_position = (world_view * vec4(position, 1)).xyz;
    frag_normal = (it_world_view * vec4(normal, 0)).xyz;
    gl_Position = world_view_projection * vec4(position, 1);
}
