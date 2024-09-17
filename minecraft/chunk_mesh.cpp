#include "chunk_mesh.h"
#include "rgame/render_window.h"
#include <iostream>

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

static void vertex_attribi_pointer(int index, int count, int type, int offset)
{
	glVertexAttribIPointer(index, count, type, sizeof(chunk_vertex), (void*)offset);
	glEnableVertexAttribArray(index);
}

void chunk_mesh::create(chunk_mesh* chunk_mesh_context)
{
	chunk_mesh_context->vbo = -1;
	chunk_mesh_context->vao = -1;

	chunk_mesh_context->uploaded = false;

	chunk_mesh_context->vertex_data = std::vector<chunk_vertex>();
}

void chunk_mesh::destroy(chunk_mesh* chunk_mesh_context)
{
	unload(chunk_mesh_context);
}

void chunk_mesh::upload(chunk_mesh* chunk_mesh_context)
{
	if (chunk_mesh_context->vertex_data.size() == 0)
		return;

	destroy(chunk_mesh_context);

	glGenVertexArrays(1, &chunk_mesh_context->vao);
	glBindVertexArray(chunk_mesh_context->vao);

	glGenBuffers(1, &chunk_mesh_context->vbo);

	glBindBuffer(GL_ARRAY_BUFFER, chunk_mesh_context->vbo);
	glBufferData(GL_ARRAY_BUFFER, chunk_mesh_context->vertex_data.size() * sizeof(chunk_vertex), chunk_mesh_context->vertex_data.data(), GL_STATIC_DRAW);

	vertex_attribi_pointer(0, 1, GL_INT, offsetof(chunk_vertex, chunk_vertex::vertex_data_0));

	chunk_mesh_context->uploaded = true;
}

void chunk_mesh::unload(chunk_mesh* chunk_mesh_context)
{
	if (!chunk_mesh_context->uploaded)
		return;

	glDeleteVertexArrays(1, &chunk_mesh_context->vao);
	glDeleteBuffers(1, &chunk_mesh_context->vbo);

	chunk_mesh_context->vbo = -1;
	chunk_mesh_context->vao = -1;

	chunk_mesh_context->uploaded = false;
}

void chunk_mesh::draw(chunk_mesh* chunk_mesh_context)
{
	if (!chunk_mesh_context->uploaded)
		return;

	glBindVertexArray(chunk_mesh_context->vao);
	glDrawArrays(GL_QUADS, 0, chunk_mesh_context->vertex_data.size());
}


chunk_vertex chunk_vertex::create(uint8_t x, uint8_t y, uint8_t z, uint8_t normal_index, uint8_t texture_x, uint8_t texture_y)
{
	uint32_t raw_data_0 = 0;

	assert(x <= POSITION_MASK);
	assert(y <= POSITION_MASK);
	assert(z <= POSITION_MASK);

	assert(normal_index < 6);

	raw_data_0 |= (x & POSITION_MASK) << X_OFFSET_BIT;
	raw_data_0 |= (y & POSITION_MASK) << Y_OFFSET_BIT;
	raw_data_0 |= (z & POSITION_MASK) << Z_OFFSET_BIT;

	raw_data_0 |= ((normal_index & NORMAL_MASK) << NORMAL_OFFSET);

	raw_data_0 |= (texture_x & UV_MASK) << X_UV_OFFSET;
	raw_data_0 |= (texture_y & UV_MASK) << Y_UV_OFFSET;

	return { raw_data_0 };
}

void chunk_vertex::set_vertex_axis(chunk_vertex* chunk_vertex_context, int index, uint8_t data)
{
	assert(index < 3);

	int shift = index * POSITION_SIZE;
	int mask = ~(POSITION_MASK << shift);

	chunk_vertex_context->vertex_data_0 = (chunk_vertex_context->vertex_data_0 & mask) | (((int)data & POSITION_MASK) << shift);
}

uint8_t chunk_vertex::get_vertex_axis(chunk_vertex* chunk_vertex_context, int index)
{
	assert(index < 3);

	int shift = index * POSITION_SIZE;

	return (chunk_vertex_context->vertex_data_0 >> shift) & POSITION_MASK;
}

void chunk_vertex::add_offset(chunk_vertex* chunk_vertex_context, ivec3b offset)
{
	for (int i = 0; i < 3; ++i)
	{
		uint8_t test = get_vertex_axis(chunk_vertex_context, i);
		set_vertex_axis(chunk_vertex_context, i, test + offset[i]);
	}
}
