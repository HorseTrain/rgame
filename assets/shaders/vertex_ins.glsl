layout (location = 0) in vec2 i_uv_0;
layout (location = 1) in vec2 i_uv_1;
layout (location = 2) in vec2 i_uv_2;

layout (location = 3) in vec3 i_position;

layout (location = 4) in vec3 i_normal;
layout (location = 5) in vec3 i_tangent;
layout (location = 6) in vec3 i_bi_tangnet;

layout (location = 7) in vec4 i_color;

layout (location = 8) in ivec4 i_weight_index;
layout (location = 9) in vec4 i_weight_data;

out vec2 uv_0;
out vec2 uv_1;
out vec2 uv_2;
out vec3 position;
out vec3 normal;
out vec3 tangent;
out vec3 bi_tangnet;
out vec4 color;