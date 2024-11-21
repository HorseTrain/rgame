#version 330 core

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D texture_atlas;
uniform float highlight;

void main()
{
    vec3 light = normalize(vec3(1));

    fragColor = texture(texture_atlas,uv) * (dot(light, normal) / 3 + .6) + vec4(highlight * .3);
}