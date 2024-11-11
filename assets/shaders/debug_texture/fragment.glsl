#version 330 core

#include "shaders/fragment_ins.glsl"

out vec4 fragmentColor;

void main()
{
    //fragmentColor = vec4(uv_0, 0, 1);

    fragmentColor = texture(texture_0, vec2(1 - uv_0.x, uv_0.y));
}