#version 330 core

#include "shaders/fragment_ins.glsl"

out vec4 fragmentColor;

void main()
{
    fragmentColor = texture(texture_0, uv_0 * texture_0_scale);
}