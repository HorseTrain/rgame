#version 330 core

#include "shaders/vertex_ins.glsl"
#include "shaders/matrix.glsl"

void main()
{
    gl_Position = vec4(i_position, 1);
}