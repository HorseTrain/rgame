#version 330 core

layout (location = 0) in vec3 i_position;
layout (location = 1) in vec2 i_uv;

uniform vec2 scale;
uniform vec2 offset;

out vec2 uv;

void main()
{
    uv = i_uv;

    vec2 position = offset + (i_position.xy * scale);

    position.y *= -1;

    gl_Position = vec4(position, 0, 1);
}