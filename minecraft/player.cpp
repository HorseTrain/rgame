#include "player.h"
#include "world.h"
#include "rgame/input_manager.h"
#include "rgame/render_window.h"
#include "rgame/game_scene.h"
#include "rgame/render_camera.h"
#include <iostream>

void player::create(player* result, world* world_context)
{
	result->world_context = world_context;

	transform::create_identity(&result->transform_context);

	result->transform_context.position.y = 10;
	result->transform_context.position.z = 20;
}

void player::destroy(player* to_destroy)
{
	//TODO
}

void player::update(player* player_context)
{
	player_update_controles(player_context);

	player_move_debug(player_context);

	player_update_camera(player_context);
}

void player::player_update_controles(player* player_context)
{
	input_manager* input_context = &player_context->world_context->input_manager_context;

	player_context->control_data.input_lr = input_manager::get_key_down(input_context, GLFW_KEY_D) - input_manager::get_key_down(input_context, GLFW_KEY_A);
	player_context->control_data.input_fb = input_manager::get_key_down(input_context, GLFW_KEY_W) - input_manager::get_key_down(input_context, GLFW_KEY_S);
	player_context->control_data.input_ud = input_manager::get_key_down(input_context, GLFW_KEY_SPACE) - input_manager::get_key_down(input_context, GLFW_KEY_LEFT_SHIFT);
}

void player::player_move_debug(player* player_context)
{
	glm::mat4 transform_matrix = transform::get_local_transform_matrix(&player_context->transform_context);

	glm::vec3 forward = transform::get_forward(transform_matrix) * -(float)player_context->control_data.input_fb;
	glm::vec3 right = transform::get_right(transform_matrix) * (float)player_context->control_data.input_lr;
	glm::vec3 up = transform::get_up(transform_matrix) * (float)player_context->control_data.input_ud;

	glm::vec3 move_direction = forward + right + up;

	player_context->transform_context.position += move_direction * get_render_window(player_context)->delta_time * 0.2f;
}

render_window* player::get_render_window(player* player_context)
{
	return player_context->world_context->raw_scene->window;
}

void player::player_update_camera(player* player_context)
{
	render_camera* camera = &player_context->world_context->raw_scene->main_camera;

	camera->_transform.position = player_context->transform_context.position;
	camera->feild_of_view = 60;
	camera->z_far = 1000;
	camera->z_near = 1;
}
