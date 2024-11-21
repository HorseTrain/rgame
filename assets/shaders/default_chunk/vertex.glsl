#version 330 core

#define POSITION_SIZE 		6
#define UV_BIT_COUNT		5
#define NORMAL_BIT_COUNT	3

#define UV_MASK				(1 << UV_BIT_COUNT) - 1
#define NORMAL_MASK			(1 << NORMAL_BIT_COUNT) - 1
#define POSITION_MASK 		(1 << POSITION_SIZE) - 1

#define X_OFFSET_BIT		POSITION_SIZE * 0
#define Y_OFFSET_BIT 		POSITION_SIZE * 1
#define Z_OFFSET_BIT 		POSITION_SIZE * 2
#define NORMAL_OFFSET		Z_OFFSET_BIT + POSITION_SIZE
#define X_UV_OFFSET			NORMAL_OFFSET + NORMAL_BIT_COUNT + (UV_BIT_COUNT * 0)
#define Y_UV_OFFSET			NORMAL_OFFSET + NORMAL_BIT_COUNT + (UV_BIT_COUNT * 1)

in int vertex_position_0;

out vec3 position;
out vec3 normal;
out vec2 uv;

uniform mat4 camera_transform;
uniform vec3 chunk_offset;

uniform vec2 texture_size;

void get_data()
{
    position = chunk_offset + vec3((vertex_position_0 >> X_OFFSET_BIT) & POSITION_MASK,(vertex_position_0 >> Y_OFFSET_BIT) & POSITION_MASK, (vertex_position_0 >> Z_OFFSET_BIT) & POSITION_MASK);
    int normal_index = (vertex_position_0 >> NORMAL_OFFSET) & NORMAL_MASK;

    switch (normal_index)
    {
        case 0: normal = vec3(-1, 0, 0); break;
        case 1: normal = vec3(1, 0, 0); break;
        case 2: normal = vec3(0, -1, 0); break;
        case 3: normal = vec3(0, 1, 0); break;
        case 4: normal = vec3(0, 0, -1); break;
        case 5: normal = vec3(0, 0, 1); break;
    }

    int uv_x = (vertex_position_0 >> X_UV_OFFSET) & UV_MASK;
    int uv_y = (vertex_position_0 >> Y_UV_OFFSET) & UV_MASK;

    uv = vec2(uv_x, uv_y) / texture_size;
}

void main()
{
    get_data();

    gl_Position = camera_transform * vec4(position, 1);
}