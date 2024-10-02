#version 330 core

#include "shaders/fragment_ins.glsl"

out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(1, 0, 0, 1);
}