#include "GL/glew.h"
#include "glm/detail/type_vec3.hpp"
#include "glm/glm.hpp"
#include "basic_render_mesh.h"

static  void vertex_attrib_array(int index, size_t offset, int type, int count)
{
	glVertexAttribPointer(index, count, type, false, sizeof(basic_render_vertex), (const void*)offset);
	glEnableVertexAttribArray(index);
}

void basic_render_mesh::create(basic_render_mesh* result, uint32_t primitive_type)
{
	result->vertex_array_buffer = -1;
	result->index_array_buffer = -1;
	result->vertex_buffer_object = -1;

	result->uploaded = false;

	result->primitive_type = primitive_type;

	result->vertices = std::vector<basic_render_vertex>();
	result->indices = std::vector<uint16_t>();
}

void basic_render_mesh::upload(basic_render_mesh* mesh)
{
	if (mesh->uploaded)
	{
		basic_render_mesh::destroy(mesh);
	}

	uint32_t vao;
	uint32_t vbo;
	uint32_t ibo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(uint16_t), mesh->indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(basic_render_vertex), mesh->vertices.data(), GL_STATIC_DRAW);

	vertex_attrib_array(0, offsetof(basic_render_vertex, basic_render_vertex::position), GL_FLOAT, 3);
	vertex_attrib_array(1, offsetof(basic_render_vertex, basic_render_vertex::normal), GL_FLOAT, 3);
	vertex_attrib_array(2, offsetof(basic_render_vertex, basic_render_vertex::uv), GL_FLOAT, 2);

	mesh->vertex_array_buffer = vao;
	mesh->index_array_buffer = ibo;
	mesh->vertex_buffer_object = vbo;

	mesh->uploaded = true;
}

void basic_render_mesh::destroy(basic_render_mesh* mesh)
{
	mesh->uploaded = false;

	if (mesh->vertex_array_buffer != -1)
		glDeleteVertexArrays(1, &mesh->vertex_array_buffer);

	if (mesh->index_array_buffer != -1)
		glDeleteBuffers(1, &mesh->index_array_buffer);

	if (mesh->vertex_buffer_object != -1)
		glDeleteBuffers(1, &mesh->vertex_buffer_object);

	mesh->vertex_array_buffer = -1;
	mesh->index_array_buffer = -1;
	mesh->vertex_buffer_object = -1;
}

void basic_render_mesh::bind(basic_render_mesh* mesh)
{
	if (!mesh->uploaded)
	{
		basic_render_mesh::upload(mesh);
	}

	glBindVertexArray(mesh->vertex_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_array_buffer);
}

void basic_render_mesh::draw(basic_render_mesh* mesh)
{
	bind(mesh);

	glDrawElements(mesh->primitive_type, mesh->indices.size(), GL_UNSIGNED_SHORT, 0);
}

void basic_render_mesh::mark_for_reupload(basic_render_mesh* mesh)
{
	destroy(mesh);
}

void basic_render_mesh::calculate_normals(basic_render_mesh* mesh)
{
	if (mesh->primitive_type != GL_TRIANGLES)
		return;

	assert(mesh->indices.size() % 3 == 0);

	for (int i = 0; i < mesh->vertices.size(); ++i)
	{
		mesh->vertices[i].normal = glm::vec3(0);
	}

	for (int i = 0; i < mesh->indices.size(); i += 3)
	{
		basic_render_vertex* vertices[3];

		bool invalid_triangle = false;

		for (int v = 0; v < 3; ++v)
		{
			int vertex_index = mesh->indices[i + v];

			if (vertex_index >= mesh->vertices.size())
			{
				invalid_triangle = true;

				break;
			}

			vertices[v] = &mesh->vertices[vertex_index];
		}

		if (invalid_triangle)
			continue;

		glm::vec3 normal_temp = glm::cross(vertices[1]->position - vertices[0]->position, vertices[2]->position - vertices[0]->position);

		for (int v = 0; v < 3; ++v)
		{
			vertices[v]->normal += normal_temp;
		}
	}

	for (int i = 0; i < mesh->vertices.size(); ++i)
	{
		basic_render_vertex* working_vertex = &mesh->vertices[i];

		working_vertex->normal /= glm::length(working_vertex->normal);
	}
}
