#include "maze_solver.h"
#include "mario/render/static_render_mesh.h"
#include "rgame/r_math.h"

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
#include "maze_solver.h"

void maze_solver::create(maze_solver* result, maze* maze_context)
{
	transform::create_identity(&result->_transform);

	result->maze_context = maze_context;
}

void maze_solver::start(maze_solver* maze_solver_context)
{
	maze_solver_context->time = 0;
}

static glm::vec2 get_cell_location(maze_cell* cell)
{
	return glm::vec2(cell->x, cell->y);
}

void maze_solver::update(maze_solver* maze_solver_context)
{
	render_camera* working_camera = &maze_solver_context->game_object_context->level_context->working_camera;
	render_window* working_window = maze_solver_context->game_object_context->game_context->window_context;
	render_surface* working_surface = lifetime_memory_manager::get_allocation<render_surface>(&maze_solver_context->game_object_context->game_context->memory, "debug_surface_surface");

	float* time = &maze_solver_context->time;

	*time += working_window->delta_time * 0.05f;

	if (*time + 1 >= maze_solver_context->maze_context->solution.size())
	{
		*time = 0;
	}

	int solution_count_part = (int)*time;
	float decimal = *time - solution_count_part;

	glm::vec2 first = get_cell_location(maze_solver_context->maze_context->solution[solution_count_part]);
	glm::vec2 second = get_cell_location(maze_solver_context->maze_context->solution[solution_count_part + 1]);

	glm::vec3 final_position;
	glm::vec3 first_position = { first.x, 0, first.y };
	glm::vec3 second_position = { second.x, 0, second.y };

	lerp((float*)&final_position, (float*)&first_position, (float*)&second_position, decimal, 3);

	maze_solver_context->_transform.position = (final_position * 5.0f) + glm::vec3(2.5f);

	maze_solver_context->_transform.position.y = 1;

	float aspect = render_window::get_aspect_ratio(working_window);

	render_surface::set_data(working_surface, "object_transform", transform::get_local_transform_matrix(&maze_solver_context->_transform), render_surface_data_type::mat4x4);
	render_surface::set_data(working_surface, "camera_transform", render_camera::get_view_matrix(working_camera, aspect), render_surface_data_type::mat4x4);

	render_surface::use(working_surface);

	static_render_mesh::upload(maze_solver_context->mesh);
	static_render_mesh::bind(maze_solver_context->mesh);

	static_render_mesh::generic_draw(maze_solver_context->mesh);
}

void maze_solver::destroy(maze_solver* maze_solver_context)
{
	static_render_mesh::destroy(maze_solver_context->mesh);
	delete maze_solver_context->mesh;
}
