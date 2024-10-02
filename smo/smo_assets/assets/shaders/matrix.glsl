vec3 transform_point(mat4 transform, vec3 point)
{
    return (transform * vec4(point, 1)).xyz;
}

vec3 transform_direction(mat4 transform, vec3 direction)
{
    return transform_point(inverse(transpose(transform)), direction);
}