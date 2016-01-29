#version 330 core

smooth in vec3 frag_position;
smooth in vec3 frag_normal;

out vec4 output_color;

void main()
{
    vec3 to_camera = -normalize(frag_position);
    float ambient = 0.2;
    float diffuse = 0.8 * dot(to_camera, frag_normal);
    output_color = (ambient + diffuse) * vec4(0.3, 1.0, 0.4, 1.0); 
}
