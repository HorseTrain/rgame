#version 330 core

#include "shaders/vertex_ins.glsl"
#include "shaders/matrix.glsl"

void set_outs()
{
    uv_0 = i_uv_0;
    uv_1 = i_uv_1;
    uv_2 = i_uv_2;

    mat4 object_transform = mat4(1);

    position = transform_point(object_transform, i_position);

    normal = transform_direction(object_transform, i_normal);
    tangent = transform_direction(object_transform, i_tangent);
    bi_tangnet = transform_direction(object_transform, i_bi_tangnet);

    color = i_color;
}

void main()
{
    set_outs();

    gl_Position = vec4(position, 1);
}