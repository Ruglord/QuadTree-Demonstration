#version 330 core

layout (location = 0) in vec4 values;
layout (location = 2) in vec3 color;
layout (location = 3) in mat4 transform;


out vec2 texCoord;
out vec3 shade;

void main()
{
    gl_Position =transform*vec4(values.xy,1,1);
    texCoord = values.ba;
    shade = color;

}
