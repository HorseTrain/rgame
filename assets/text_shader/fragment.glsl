#version 330 core

out vec4 fragColor;

in vec2 uv;
uniform sampler2D texture_source;

void main()
{
    fragColor = texture(texture_source, uv);
}