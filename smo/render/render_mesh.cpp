#include "render_mesh.h"

void render_mesh::create(render_mesh* result, int primitive_type, int vertex_count, int index_count)
{
	result->primitive_type = primitive_type;

	array_container<render_vertex>::create(&result->vertices, vertex_count);
	array_container<uint16_t>::create(&result->indices, index_count);
}

void render_mesh::destroy(render_mesh* render_mesh_context)
{
	array_container<render_vertex>::destroy(&render_mesh_context->vertices);
	array_container<uint16_t>::destroy(&render_mesh_context->indices);

	unload(render_mesh_context);
}

void render_mesh::upload(render_mesh* render_mesh_context)
{
	//TODO
}

void render_mesh::unload(render_mesh* render_mesh_context)
{
	//TODO
}

void render_mesh::draw(render_mesh* render_mesh_context)
{
	//TODO
}
