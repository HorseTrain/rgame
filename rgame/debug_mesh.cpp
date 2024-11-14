#include "debug_mesh.h"
#include "rgame/render_window.h"

static bool is_uploaded(debug_mesh* debug_mesh_context)
{
	return debug_mesh_context->gl_handles[0] != -1;
}

static void negate_buffers(debug_mesh* debug_mesh_context)
{
	memset(debug_mesh_context->gl_handles, -1, sizeof(debug_mesh::gl_handles));
}

void debug_mesh::create(debug_mesh* result, int primitive_mode)
{
	result->vertices = new std::vector<debug_mesh_vertex>();
	result->indices = new std::vector<uint16_t>();

	result->primitive_mode = primitive_mode;

	negate_buffers(result);
}

void debug_mesh::destroy(debug_mesh* debug_mesh_context)
{
	unload(debug_mesh_context);

	delete debug_mesh_context->indices;
	delete debug_mesh_context->vertices;
}

static void vertex_attrib_pointer(int index, int count, int type, int offset)
{
	glVertexAttribPointer(index, count, GL_FLOAT, false, sizeof(debug_mesh_vertex), (void*)offset);
	glEnableVertexAttribArray(index);
}

void debug_mesh::upload(debug_mesh* to_upload)
{
	unload(to_upload);

	glGenVertexArrays(1, &to_upload->gl_handles[0]);
	glGenBuffers(1, &to_upload->gl_handles[1]);
	glGenBuffers(1, &to_upload->gl_handles[2]);

	glBindVertexArray(to_upload->gl_handles[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, to_upload->gl_handles[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, to_upload->indices->size() * sizeof(uint16_t), to_upload->indices->data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, to_upload->gl_handles[2]);
	glBufferData(GL_ARRAY_BUFFER, to_upload->vertices->size() * sizeof(debug_mesh_vertex), to_upload->vertices->data(), GL_STATIC_DRAW);

	vertex_attrib_pointer(0, 3, GL_FLOAT, offsetof(debug_mesh_vertex, debug_mesh_vertex::position));
	vertex_attrib_pointer(1, 2, GL_FLOAT, offsetof(debug_mesh_vertex, debug_mesh_vertex::uv));
}

void debug_mesh::unload(debug_mesh* to_upload)
{
	if (!is_uploaded(to_upload))
	{
		return;
	}

	glDeleteVertexArrays(1, &to_upload->gl_handles[0]);
	glDeleteBuffers(1, &to_upload->gl_handles[1]);
	glDeleteBuffers(1, &to_upload->gl_handles[2]);

	negate_buffers(to_upload);
}

void debug_mesh::bind(debug_mesh* to_bind)
{
	glGenVertexArrays(1, &to_bind->gl_handles[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, to_bind->gl_handles[1]);
}

void debug_mesh::generic_draw(debug_mesh* to_draw)
{
	if (!is_uploaded(to_draw))
	{
		upload(to_draw);
	}

	bind(to_draw);

	glDrawElements(to_draw->primitive_mode, to_draw->indices->size(), GL_UNSIGNED_SHORT, nullptr);
}

void debug_mesh::create_debug_quad(debug_mesh** result)
{
	debug_mesh* quad = new debug_mesh;

	debug_mesh::create(quad, GL_QUADS);

	quad->vertices->push_back({ {0,0,0}, {0,0} });
	quad->vertices->push_back({ {1,0,0}, {1,0} });
	quad->vertices->push_back({ {1,1,0}, {1,1} });
	quad->vertices->push_back({ {0,1,0}, {0,1} });

	for (int i = 0; i < 4; ++i)
	{
		quad->indices->push_back(i);
	}

	*result = quad;
}
