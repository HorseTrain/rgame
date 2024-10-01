#include "static_render_mesh.h"
#include "rgame/render_window.h"

#include "rgame/glm/glm.hpp"

static void vertex_attrib_pointer(int index, int size,int type,int offset)
{
	switch (type)
	{
	case GL_FLOAT:
	case GL_DOUBLE:
	{
		glVertexAttribPointer(index, size, type, false, sizeof(render_vertex), (void*)offset);
	}; break;

	default: 
	{
		glVertexAttribIPointer(index, size, type, sizeof(render_vertex), (void*)offset);
	}; break;
	}

	glEnableVertexAttribArray(index);
}

static void set_attrib_data()
{
	vertex_attrib_pointer(0, 2, GL_FLOAT, offsetof(render_vertex, render_vertex::uv_0));
	vertex_attrib_pointer(1, 2, GL_FLOAT, offsetof(render_vertex, render_vertex::uv_1));

	vertex_attrib_pointer(2, 3, GL_FLOAT, offsetof(render_vertex, render_vertex::position));
	vertex_attrib_pointer(3, 3, GL_FLOAT, offsetof(render_vertex, render_vertex::normal));
	vertex_attrib_pointer(4, 3, GL_FLOAT, offsetof(render_vertex, render_vertex::tangent));
	vertex_attrib_pointer(5, 3, GL_FLOAT, offsetof(render_vertex, render_vertex::bi_tangnet));

	vertex_attrib_pointer(6, 4, GL_INT, offsetof(render_vertex, render_vertex::weight_index));
	vertex_attrib_pointer(7, 4, GL_FLOAT, offsetof(render_vertex, render_vertex::weight_data));

	vertex_attrib_pointer(8, 4, GL_FLOAT, offsetof(render_vertex, render_vertex::color));
}

void static_render_mesh::create(static_render_mesh* result, int primitive_type, int vertex_count, int index_count)
{
	result->primitive_type = primitive_type;

	if (vertex_count != 0)
		array_container<render_vertex>::create(&result->vertices, vertex_count);

	if (index_count != 0)
		array_container<uint16_t>::create(&result->indices, index_count);

	memset(result->gl_buffers, -1, sizeof(static_render_mesh::gl_buffers));
}

void static_render_mesh::destroy(static_render_mesh* render_mesh_context)
{
	array_container<render_vertex>::destroy(&render_mesh_context->vertices);
	array_container<uint16_t>::destroy(&render_mesh_context->indices);

	unload(render_mesh_context);
}

void static_render_mesh::upload(static_render_mesh* render_mesh_context)
{
	if (is_uploaded(render_mesh_context))
		return;
	
	unload(render_mesh_context);

	GLuint* vao = (GLuint*)&render_mesh_context->gl_buffers[0];
	GLuint* vbo = (GLuint*)&render_mesh_context->gl_buffers[1];
	GLuint* ibo = (GLuint*)&render_mesh_context->gl_buffers[2];

	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	glGenBuffers(1, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, render_mesh_context->vertices.count * sizeof(render_vertex), render_mesh_context->vertices.data, GL_STATIC_DRAW);

	set_attrib_data();

	glGenBuffers(1, ibo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_mesh_context->indices.count * sizeof(uint16_t), render_mesh_context->indices.data, GL_STATIC_DRAW);
}

void static_render_mesh::unload(static_render_mesh* render_mesh_context)
{
	if (render_mesh_context->gl_buffers[0] != -1)
		glDeleteVertexArrays(1, (GLuint*) & render_mesh_context->gl_buffers[0]);

	for (int i = 1; i < 3; ++i)
	{
		int* buffer_ptr = &render_mesh_context->gl_buffers[i];

		if (*buffer_ptr == -1)
			continue;

		glDeleteBuffers(1, (GLuint*)buffer_ptr);
	}

	memset(render_mesh_context->gl_buffers, -1, sizeof(static_render_mesh::gl_buffers));
}

void static_render_mesh::draw(static_render_mesh* render_mesh_context)
{
	upload(render_mesh_context);
	bind(render_mesh_context);

	glDrawElements(render_mesh_context->primitive_type, render_mesh_context->indices.count, GL_UNSIGNED_SHORT, 0);
}

bool static_render_mesh::is_uploaded(static_render_mesh* render_mesh_context)
{
	for (int i = 0; i < 3; ++i)
	{
		if (render_mesh_context->gl_buffers[i] == -1)
			return false;
	}

	return true;
}

void static_render_mesh::bind(static_render_mesh* render_mesh_context)
{
	glBindVertexArray(render_mesh_context->gl_buffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,render_mesh_context->gl_buffers[2]);
}

void static_render_mesh::calculate_normals(static_render_mesh* static_render_mesh_context)
{
	if (static_render_mesh_context->primitive_type != GL_TRIANGLES)
		return;

	static_render_mesh::unload(static_render_mesh_context);

	array_container<uint16_t> tris = static_render_mesh_context->indices;
	array_container<render_vertex> vert = static_render_mesh_context->vertices;

	for (int i = 0; i < tris.count; i += 3)
	{
		glm::vec3 p0 = vert[tris[i + 0]].position;
		glm::vec3 p1 = vert[tris[i + 1]].position;
		glm::vec3 p2 = vert[tris[i + 2]].position;

		glm::vec3 new_normal = glm::cross(p2 - p0, p1 - p0);

		vert[tris[i + 0]].normal += new_normal;
		vert[tris[i + 1]].normal += new_normal;
		vert[tris[i + 2]].normal += new_normal;
	}

	for (int i = 0; i < vert.count; ++i)
	{
		vert[i].normal = glm::normalize(vert[i].normal);
	}
}

void static_render_mesh::create_triangle(static_render_mesh* result)
{
	create(result, GL_TRIANGLES, 3, 3);

	for (int i = 0; i < 3; ++i)
	{
		result->indices[i] = i;
	}

	result->vertices[0] = render_vertex::create_position(0, 0, 0);
	result->vertices[1] = render_vertex::create_position(1, 0, 0);
	result->vertices[2] = render_vertex::create_position(1, 1, 0);
}
