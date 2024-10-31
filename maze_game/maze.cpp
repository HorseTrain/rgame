#include "maze.h"
#include "maze_level.h"
#include "mario/render/static_render_mesh.h"

static render_vertex from_position(float x, float y, float z)
{
	render_vertex result;

	result.position = { x, y, z };

	return result;
}

void maze::create(maze** maze_context, maze_level* maze_level_context)
{
	maze* result = new maze;

	result->maze_level_context = maze_level_context;
	result->mesh_context = nullptr;

	*maze_context = result;
}

void maze::destroy(maze* maze_context)
{
	delete maze_context;
}

static void destroy_current_mesh(static_render_mesh* static_render_mesh_context)
{
	if (static_render_mesh_context == nullptr)
		return;

	static_render_mesh::destroy(static_render_mesh_context);

	delete static_render_mesh_context;
}

void maze::create_debug_triangle(maze* maze_context)
{
	destroy_current_mesh(maze_context->mesh_context);

	static_render_mesh* result = new static_render_mesh;

	std::vector<render_vertex> vertices = {

		from_position(0, 0, 0),
		from_position(1, 0, 0),
		from_position(1, 1, 0)

	};

	std::vector<uint16_t> indices = {
		0, 1, 2
	};

	static_render_mesh::create(maze_context->mesh_context, 3, 3, GL_TRIANGLES);

	fast_array< uint16_t>::copy_from(&result->indecies, indices.data());
	fast_array< render_vertex>::copy_from(&result->vertices, vertices.data());

	maze_context->mesh_context = result;
};
