#include "static_render_mesh.h"
#include "mario.h"

static void vertex_attrib_pointer(int index, int count, int type, int offset)
{
	glVertexAttribPointer(index, count, GL_FLOAT, false, sizeof(render_vertex), (void*)offset);
	glEnableVertexAttribArray(index);
}

static void vertex_attrib_i_pointer(int index, int count, int type, int offset)
{
	glVertexAttribIPointer(index, count, GL_UNSIGNED_INT, sizeof(render_vertex), (void*)offset);
	glEnableVertexAttribArray(index);
}

static void negate_buffers(static_render_mesh* static_render_mesh_context)
{
	static_render_mesh_context->gl_handles[0] = -1;
	static_render_mesh_context->gl_handles[1] = -1;
	static_render_mesh_context->gl_handles[2] = -1;
}

void static_render_mesh::upload(static_render_mesh* static_render_mesh_context)
{
	if (is_uploaded(static_render_mesh_context))
	{
		return;
	}

	uint32_t* vao = &static_render_mesh_context->gl_handles[0];
	uint32_t* ibo = &static_render_mesh_context->gl_handles[1];
	uint32_t* vbo = &static_render_mesh_context->gl_handles[2];

	glGenVertexArrays(1,vao);

	glBindVertexArray(*vao);

	glGenBuffers(1, ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_render_mesh_context->indecies.count * sizeof(uint16_t), static_render_mesh_context->indecies.data, GL_STATIC_DRAW);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER , *vbo);
	glBufferData(GL_ARRAY_BUFFER, static_render_mesh_context->vertices.count * sizeof(render_vertex), static_render_mesh_context->vertices.data, GL_STATIC_DRAW);

	vertex_attrib_pointer(0, 2, GL_FLOAT, offsetof(render_vertex, render_vertex::uv_0));
	vertex_attrib_pointer(1, 2, GL_FLOAT, offsetof(render_vertex, render_vertex::uv_1));
	vertex_attrib_pointer(2, 2, GL_FLOAT, offsetof(render_vertex, render_vertex::uv_2));

	vertex_attrib_pointer(3, 3, GL_FLOAT, offsetof(render_vertex, render_vertex::position));
	vertex_attrib_pointer(4, 3, GL_FLOAT, offsetof(render_vertex, render_vertex::normal));
	vertex_attrib_pointer(5, 3, GL_FLOAT, offsetof(render_vertex, render_vertex::tangent));
	vertex_attrib_pointer(6, 3, GL_FLOAT, offsetof(render_vertex, render_vertex::bitangent));

	vertex_attrib_pointer(7, 4, GL_FLOAT, offsetof(render_vertex, render_vertex::color));
	vertex_attrib_i_pointer(8, 4, GL_FLOAT, offsetof(render_vertex, render_vertex::weight_indices));
	vertex_attrib_pointer(9, 4, GL_FLOAT, offsetof(render_vertex, render_vertex::weights));
}

void static_render_mesh::unload(static_render_mesh* static_render_mesh_context)
{
	if (!is_uploaded(static_render_mesh_context))
	{
		return;
	}

	glDeleteVertexArrays(1, &static_render_mesh_context->gl_handles[0]);
	glDeleteBuffers(2, &static_render_mesh_context->gl_handles[1]);

	negate_buffers(static_render_mesh_context);
}

bool static_render_mesh::is_uploaded(static_render_mesh* static_render_mesh_context)
{
	bool current_result = static_render_mesh_context->gl_handles[0] != -1;

	for (int i = 0; i < sizeof(static_render_mesh::gl_handles) / sizeof(uint32_t); ++i)
	{
		if ((static_render_mesh_context->gl_handles[i] == -1 && current_result) || (static_render_mesh_context->gl_handles[i] != -1 && !current_result))
		{
			assert(false);

			throw 0;
		}
	}

	return current_result;
}

void static_render_mesh::bind(static_render_mesh* static_render_mesh_context)
{
	upload(static_render_mesh_context);

	uint32_t vao = static_render_mesh_context->gl_handles[0];
	uint32_t ibo = static_render_mesh_context->gl_handles[1];

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void static_render_mesh::create(static_render_mesh* result, int index_count, int vertex_count, int primative_type)
{
	fast_array<uint16_t>::create(&result->indecies, index_count);
	fast_array<render_vertex>::create(&result->vertices, vertex_count);

	result->primative_type = primative_type;

	negate_buffers(result);
}

void static_render_mesh::destroy(static_render_mesh* static_render_mesh_context)
{
	unload(static_render_mesh_context);

	delete static_render_mesh_context->indecies.data;
	delete static_render_mesh_context->vertices.data;
}

void static_render_mesh::create_debug_triangle(static_render_mesh* result)
{
	static_render_mesh::create(result, 3, 3, GL_TRIANGLES);

	result->vertices[0].position = glm::vec3(0, 0, 0);
	result->vertices[1].position = glm::vec3(1, 0, 0);
	result->vertices[2].position = glm::vec3(1, 1, 0);

	result->indecies[0] = 0;
	result->indecies[1] = 1;
	result->indecies[2] = 2;
}

void static_render_mesh::create_debug_quad(static_render_mesh* result)
{
	static_render_mesh::create(result, 4, 4, GL_QUADS);

	result->vertices[0].position = glm::vec3(0, 0, 0);
	result->vertices[1].position = glm::vec3(1, 0, 0);
	result->vertices[2].position = glm::vec3(1, 0, 1);
	result->vertices[3].position = glm::vec3(0, 0, 1);

	result->vertices[0].uv_0 = glm::vec2(0, 0);
	result->vertices[1].uv_0 = glm::vec2(1, 0);
	result->vertices[2].uv_0 = glm::vec2(1, 1);
	result->vertices[3].uv_0 = glm::vec2(0, 1);

	result->indecies[0] = 0;
	result->indecies[1] = 1;
	result->indecies[2] = 2;
	result->indecies[3] = 3;
}

void static_render_mesh::create_debug_plane(static_render_mesh* result, float size)
{
	static_render_mesh::create(result, 4, 4, GL_QUADS);

	result->vertices[0].position = glm::vec3(0.5f, 0, 0.5f);
	result->vertices[1].position = glm::vec3(-0.5f, 0, 0.5f);
	result->vertices[2].position = glm::vec3(-0.5f, 0, -0.5f);
	result->vertices[3].position = glm::vec3(0.5f, 0, -0.5f);

	result->vertices[0].uv_0 = glm::vec2(0, 0);
	result->vertices[1].uv_0 = glm::vec2(1, 0);
	result->vertices[2].uv_0 = glm::vec2(1, 1);
	result->vertices[3].uv_0 = glm::vec2(0, 1);

	for (int i = 0; i < result->indecies.count; ++i)
	{
		result->vertices[i].position *= size;
		result->indecies[i] = i;
	}
}

void static_render_mesh::create_debug_cube(static_render_mesh* result, float size)
{
	static_render_mesh::create(result, 16,8, GL_QUADS);

	result->vertices[0].position = glm::vec3(0.5, -0.5, 0.5);
	result->vertices[1].position = glm::vec3(-0.5, -0.5, 0.5);
	result->vertices[2].position = glm::vec3(-0.5, -0.5, -0.5);
	result->vertices[3].position = glm::vec3(0.5, -0.5, -0.5);

	result->vertices[4].position = glm::vec3(0.5, 0.5, 0.5);
	result->vertices[5].position = glm::vec3(-0.5, 0.5, 0.5);
	result->vertices[6].position = glm::vec3(-0.5, 0.5, -0.5);
	result->vertices[7].position = glm::vec3(0.5, 0.5, -0.5);

	result->indecies[0] = 0;
	result->indecies[1] = 1;
	result->indecies[2] = 5;
	result->indecies[3] = 4;

	result->indecies[4] = 2;
	result->indecies[5] = 3;
	result->indecies[6] = 7;
	result->indecies[7] = 6;
}

void static_render_mesh::generic_draw(static_render_mesh* result)
{
	glDrawElements(result->primative_type, result->indecies.count, GL_UNSIGNED_SHORT, nullptr);
}
