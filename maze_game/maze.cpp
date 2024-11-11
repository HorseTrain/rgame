#include "maze.h"
#include "maze_level.h"
#include "mario/render/static_render_mesh.h"
#include "rgame/render_texture.h"
#include "rgame/pixel.h"
#include "mario/asset_helper.h"
#include "rgame/lifetime_memory_manager.h"
#include "mario/game_object.h"
#include "mario/game.h"
#include "maze_generator.h"

static render_vertex from_position(float x, float y, float z)
{
	render_vertex result;

	result.position = { x, y, z };

	return result;
}

static void destroy_current_maze_mesh(maze* maze_context)
{
	if (maze_context->maze_mesh != nullptr)
	{
		static_render_mesh::destroy(maze_context->maze_mesh);

		delete maze_context->maze_mesh;
	}
}

void maze::create(maze* result, maze_level* maze_level_context)
{
	result->maze_level_context = maze_level_context;

	result->cells = nullptr;
	result->maze_mesh = nullptr;

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
	destroy_current_maze_mesh(maze_context);

	if (maze_context->cells == nullptr)
		return;

	delete maze_context->cells;

	for (auto i : maze_context->walls)
	{
		delete i.second;
	}

	maze_context->walls.clear();

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

	memset(new_cells, 0, sizeof(maze_cell) * maze_cell_count);

	maze_context->cells = new_cells;

	for (int i = 0; i < maze_cell_count; ++i)
	{
		new_cells[i].maze_context = maze_context;
	}

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			maze_cell* working_cell = maze::get_cell(maze_context, x, y);

			working_cell->x = x;
			working_cell->y = y;

			maze_cell::find_neighbors(working_cell);
		}
	}

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			wall_key keys[] = { {x, y, EAST_WEST} , {x + 1, y, NORTH_SOUTH}, { x, y + 1, EAST_WEST }, { x, y, NORTH_SOUTH } };

			maze_cell* working_cell = maze::get_cell(maze_context, x, y);

			for (int i = 0; i < 4; ++i)
			{
				wall_key working_key = keys[i];

				if (maze_context->walls.find(working_key) != maze_context->walls.end())
				{
					working_cell->walls[i] = maze_context->walls[working_key];

					continue;
				}

				maze_wall* new_wall = new maze_wall();

				working_cell->walls[i] = new_wall;

				maze_context->walls[working_key] = new_wall;
			}
		}
	}

	maze_generator maze_generator_context;

	maze_generator::create(&maze_generator_context, maze_context);
	maze_generator::generate_randomizd_depth_first_search(&maze_generator_context);
}

static int create_multiplier(int source, int square_size)
{
	return (source * square_size) + source + 1;
}

static void draw_line(render_texture* texture, int x, int y, int length,bool is_x_axis, rgba_i8 color)
{
	for (int i = 0; i < length; ++i)
	{
		if (is_x_axis)
		{
			x++;
		}
		else
		{
			y++;
		}

		*rgba_i8::get_pixel((rgba_i8*)texture->texture_buffer, x, y, texture->width) = color;
	}
}

static void draw_line(render_texture* texture, glm::vec2 p0, glm::vec2 p1, rgba_i8 color)
{
	glm::vec2 direction = p1 - p0;
	float length = glm::length(direction);
	direction /= length;

	glm::vec2 current = p0;

	for (int i = 0; i < length; ++i)
	{
		current += direction;

		int x = (int)current.x;
		int y = (int)current.y;

		*rgba_i8::get_pixel((rgba_i8*)texture->texture_buffer, x, y, texture->width) = color;
	}
}

void maze::generate_maze_texture(render_texture** render_texture_context, maze* maze_context)
{
	render_texture* result = new render_texture;

	int maze_width = maze_context->cell_width;
	int maze_height = maze_context->cell_height;

	render_texture::create_empty(
		result, 
		create_multiplier(maze_width, CELL_SIZE),
		create_multiplier(maze_height, CELL_SIZE),
		sizeof(rgba_i8)
	);

	for (auto i : maze_context->walls)
	{
		wall_key key = i.first;
		maze_wall* working_wall = i.second;

		if (working_wall->is_open)
			continue;

		int px = key.base_x * (CELL_SIZE + 1);
		int py = key.base_y * (CELL_SIZE + 1);

		draw_line(result, px, py, CELL_SIZE, key.direction == EAST_WEST, {0, 0, 0, 0});
	}

	for (int i = 0; i < maze_context->solution.size() - 1; ++i)
	{
		draw_line(result, maze_context->solution[i], maze_context->solution[i + 1], { 255, 0, 0, 255});
	}

	*render_texture_context = result;
}

void maze::generate_maze_mesh(maze* maze_context)
{
	destroy_current_maze_mesh(maze_context);

	maze_context->maze_mesh = new static_render_mesh;

	for (auto i : maze_context->walls)
	{
		wall_key key = i.first;
		maze_wall* working_wall = i.second;

		if (working_wall->is_open)
			continue;
	}

	static_render_mesh::create_debug_triangle(maze_context->maze_mesh);
}

void maze::render_debug_texture(maze* maze_context)
{
	//
}

maze_cell* maze::get_cell(maze* maze_context, int x, int y)
{
	if (x >= maze_context->cell_width || y >= maze_context->cell_height ||
		x < 0 || 
		y < 0)
	{
		return nullptr;
	}

	return maze_context->cells + (y * maze_context->cell_width) + x;
}

void maze::start(maze* maze_context)
{
	maze::generate_maze(maze_context, 100, 100);
	maze::generate_maze_mesh(maze_context);
}

void maze::update(maze* maze_context)
{
	render_camera* working_camera = &maze_context->maze_level_context->level_context->working_camera;
	render_window* working_window = maze_context->game_object_context->game_context->window_context;
	render_surface* working_surface = lifetime_memory_manager::get_allocation<render_surface>(&maze_context->game_object_context->game_context->memory, "debug_surface_surface");

	float aspect = render_window::get_aspect_ratio(working_window);

	if (maze_context->maze_mesh == nullptr)
		return;

	render_surface::set_data(working_surface, "object_transform", glm::mat4(1), render_surface_data_type::mat4x4);
	render_surface::set_data(working_surface, "camera_transform", render_camera::get_view_matrix(working_camera, aspect), render_surface_data_type::mat4x4);

	render_surface::use(working_surface);

	static_render_mesh::upload(maze_context->maze_mesh);
	static_render_mesh::bind(maze_context->maze_mesh);

	static_render_mesh::generic_draw(maze_context->maze_mesh);
}
