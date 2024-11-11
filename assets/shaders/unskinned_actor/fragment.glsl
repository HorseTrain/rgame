#version 330 core

#include "shaders/fragment_ins.glsl"

out vec4 fragmentColor;

void main()
{
    fragmentColor = texture(texture_0, vec2(uv_0.x, 1 - uv_0.y));
}