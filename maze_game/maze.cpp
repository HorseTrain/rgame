#include "maze.h"
#include "maze_level.h"
#include "mario/render/static_render_mesh.h"
#include "rgame/render_texture.h"
#include "rgame/pixel.h"
#include "mario/asset_helper.h"
#include "rgame/lifetime_memory_manager.h"
#include "mario/game_object.h"
#include "mario/game.h"

static render_vertex from_position(float x, float y, float z)
{
	render_vertex result;

	result.position = { x, y, z };

	return result;
}

void maze::create(maze* result, maze_level* maze_level_context)
{
	result->maze_level_context = maze_level_context;

	result->cells = nullptr;

	result->cell_width = -1;
	result->cell_height = -1;

	lifetime_memory_manager* game_memory = &result->game_object_context->game_context->memory;
	render_surface* working_render_surface = lifetime_memory_manager::get_allocation<render_surface>(game_memory, "debug_texture_surface");

	create_render_surface_copy(&result->debug_texture_surface, working_render_surface, game_memory);
}

void maze::destroy(maze* maze_context)
{
	maze::destroy_current_maze(maze_context);
}

void maze::destroy_current_maze(maze* maze_context)
{
	if (maze_context->cells == nullptr)
		return;

	delete maze_context->cells;

	maze_context->cell_width = -1;
	maze_context->cell_height = -1;
}

void maze::generate_maze(maze* maze_context, int width, int height)
{
	destroy_current_maze(maze_context);

	maze_context->cell_width = width;
	maze_context->cell_height = height;

	int maze_cell_count = width * height;

	maze_cell* new_cells = new maze_cell[maze_cell_count];

	for (int i = 0; i < maze_cell_count; ++i)
	{
		new_cells[i] = { maze_context, {true, true, true, true} };
	}

	maze_context->cells = new_cells;
}

static int create_multiplier(int source, int square_size)
{
	return (source * square_size) + source + 1;
}

void maze::generate_maze_texture(render_texture** render_texture_context, maze* maze_context)
{
	render_texture* result = new render_texture;

	render_texture::create_empty(
		result, 
		create_multiplier(maze_context->cell_width, 10), 
		create_multiplier(maze_context->cell_height, 10),
		sizeof(rgba_i8)
	);

	*render_texture_context = result;
}

void maze::render_debug_texture(maze* maze_context)
{
	render_texture* working_texture;

	render_surface* working_surface = maze_context->debug_texture_surface;

	generate_maze_texture(&working_texture, maze_context);

	working_surface->textures[0] = working_texture;

	render_surface::use(working_surface);

	static_render_mesh* triangle = new static_render_mesh();

	static_render_mesh::create_debug_quad(triangle);

	static_render_mesh::bind(triangle);
	static_render_mesh::generic_draw(triangle);

	static_render_mesh::destroy(triangle);


	delete triangle;

	render_texture::destroy(working_texture);
	delete working_texture;
}

void maze::start(maze* maze_context)
{
	maze::generate_maze(maze_context, 100, 100);
}

void maze::update(maze* maze_context)
{
	render_debug_texture(maze_context);
}
