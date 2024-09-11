#include "chunk_mesh.h"
#include "rgame/render_window.h"

static void vertex_attribi_pointer(int index, int count, int type, int offset)
{
	glVertexAttribIPointer(index, count, type, sizeof(chunk_vertex), (void*)offset);
	glEnableVertexAttribArray(index);
}

void chunk_mesh::create(chunk_mesh* chunk_mesh_context)
{
	chunk_mesh_context->vbo = -1;
	chunk_mesh_context->vertex_data = std::vector<chunk_vertex>();
}

void chunk_mesh::destroy(chunk_mesh* chunk_mesh_context)
{
	if (chunk_mesh_context->vbo != -1)
	{
		glDeleteBuffers(1, (GLuint*)&chunk_mesh_context->vbo);
	}
}

void chunk_mesh::upload(chunk_mesh* chunk_mesh_context)
{
	destroy(chunk_mesh_context);

	glGenBuffers(1, &chunk_mesh_context->vbo);

	glBindBuffer(GL_ARRAY_BUFFER, chunk_mesh_context->vbo);
	glBufferData(GL_ARRAY_BUFFER, chunk_mesh_context->vertex_data.size() * sizeof(chunk_vertex), chunk_mesh_context->vertex_data.data(), GL_STATIC_DRAW);

	vertex_attribi_pointer(0, 1, GL_INT, offsetof(chunk_vertex, chunk_vertex::vertex_data_0));
}

void chunk_mesh::unload(chunk_mesh* chunk_mesh_context)
{
	if (chunk_mesh_context->vbo == -1)
		return;

	glDeleteBuffers(1, &chunk_mesh_context->vbo);

	chunk_mesh_context->vbo = -1;
}

void chunk_mesh::draw(chunk_mesh* chunk_mesh_context)
{
	if (chunk_mesh_context->vbo == -1)
		return;

	glBindBuffer(GL_ARRAY_BUFFER,chunk_mesh_context->vbo);
	glDrawArrays(GL_QUADS, 0, chunk_mesh_context->vertex_data.size());
}

chunk_vertex chunk_vertex::create(uint8_t x, uint8_t y, uint8_t z, uint8_t normal_index, uint8_t texture_x, uint8_t texture_y)
{
	uint32_t raw_data_0 = 0;

	assert(x < 32);
	assert(y < 32);
	assert(z < 32);

	assert(normal_index < 6);

	raw_data_0 |= (x & 31) << 0;
	raw_data_0 |= (y & 31) << 5;
	raw_data_0 |= (z & 31) << 10;

	raw_data_0 |= (normal_index << 15);

	return { raw_data_0 };
}
