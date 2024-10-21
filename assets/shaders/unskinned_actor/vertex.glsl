#version 330 core

#include "shaders/vertex_ins.glsl"
#include "shaders/matrix.glsl"

uniform mat4 camera_transform;
uniform mat4 object_transform;

void set_outs()
{
    uv_0 = i_uv_0;
    uv_1 = i_uv_1;
    uv_2 = i_uv_2;

    position = transform_point(object_transform, i_position);

    normal = transform_direction(object_transform, i_normal);
    tangent = transform_direction(object_transform, i_tangent);
    bi_tangnet = transform_direction(object_transform, i_bi_tangnet);

    color = i_color;
}

void main()
{
    set_outs();

    gl_Position = camera_transform * vec4(position, 1);
}