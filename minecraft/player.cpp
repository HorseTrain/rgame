#include "player.h"
#include "world.h"
#include "rgame/input_manager.h"
#include "rgame/render_window.h"
#include "rgame/game_scene.h"
#include "rgame/render_camera.h"
#include "rgame/render_window.h"
#include <iostream>

void player::create(player* result, world* world_context)
{
	result->world_context = world_context;

	transform::create_identity(&result->transform_context);

	result->transform_context.position.y = 0;
	result->transform_context.position.z = 0;

	result->mouse_is_locked = false;

	result->camera_rotation_x = 0;
	result->camera_rotation_y = 0;
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

	if (input_manager::get_key_pressed(input_context, GLFW_KEY_ESCAPE))
	{
		if (player_context->mouse_is_locked)
			player_context->mouse_is_locked = false;
		else
			player_context->mouse_is_locked = true;

		if (player_context->mouse_is_locked)
		{
			glfwSetInputMode(player::get_render_window(player_context)->raw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(player::get_render_window(player_context)->raw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	player_context->control_data.input_lr = input_manager::get_key_down(input_context, GLFW_KEY_D) - input_manager::get_key_down(input_context, GLFW_KEY_A);
	player_context->control_data.input_fb = input_manager::get_key_down(input_context, GLFW_KEY_W) - input_manager::get_key_down(input_context, GLFW_KEY_S);
	player_context->control_data.input_ud = input_manager::get_key_down(input_context, GLFW_KEY_SPACE) - input_manager::get_key_down(input_context, GLFW_KEY_LEFT_SHIFT);

	if (player_context->mouse_is_locked)
	{
		player_context->camera_rotation_x = input_context->mouse_x / 1000.0f;
		player_context->camera_rotation_y = input_context->mouse_y / 1000.0f;
	}

	player_context->transform_context.rotation = glm::angleAxis(player_context->camera_rotation_y, glm::vec3(1, 0, 0)) * glm::angleAxis(player_context->camera_rotation_x, glm::vec3(0, 1, 0));
}

void player::player_move_debug(player* player_context)
{
	glm::mat4 transform_matrix = transform::get_local_transform_matrix(&player_context->transform_context);

	glm::vec3 forward = transform::get_forward(transform_matrix) * -(float)player_context->control_data.input_fb;
	glm::vec3 right = transform::get_right(transform_matrix) * (float)player_context->control_data.input_lr;
	glm::vec3 up = glm::vec3(0, 1, 0) * (float)player_context->control_data.input_ud;

	glm::vec3 move_direction = forward + right + up;

	player_context->transform_context.position += move_direction * get_render_window(player_context)->delta_time * 5.0f;
}

render_window* player::get_render_window(player* player_context)
{
	return player_context->world_context->raw_scene->window;
}

void player::player_update_camera(player* player_context)
{
	render_camera* camera = &player_context->world_context->raw_scene->main_camera;

	camera->_transform.position = player_context->transform_context.position;
	camera->_transform.rotation = player_context->transform_context.rotation;

	camera->feild_of_view = 60;
	camera->z_far = 1000;
	camera->z_near = 1;
}
