#include "player.h"
#include "rgame/render_camera.h"
#include "rgame/transform.h"
#include "rgame/render_window.h"
#include "game.h"
#include "world.h"

#include "chunk.h"

void player::create(player* player_context, world* world_context)
{
	player_context->world_context = world_context;

	player_context->mode = player_mode_spectator;

	transform::create_identity(&player_context->transform_context);

	player_context->transform_context.position = glm::vec3(0, 3, 10);
}

void player::destroy(player* player_context)
{
	//
}

static void player_update_spectator(player* player_context)
{
	render_camera* camera = &player_context->world_context->main_camera;
	render_window* render_window = player_context->world_context->game_context->window_context;
	input_manager* input_manager_context = &player_context->world_context->game_context->input_manager_context;

	float delta_time = render_window->delta_time;

	int forward_back_axis = -input_manager::get_key_down(input_manager_context, GLFW_KEY_W) + input_manager::get_key_down(input_manager_context, GLFW_KEY_S);
	int left_right_axis = input_manager::get_key_down(input_manager_context, GLFW_KEY_D) - input_manager::get_key_down(input_manager_context, GLFW_KEY_A);
	int up_down_axis = input_manager::get_key_down(input_manager_context, GLFW_KEY_SPACE) - input_manager::get_key_down(input_manager_context,	GLFW_KEY_LEFT_SHIFT);

	glm::mat4 current_transform_matrix = transform::get_local_transform_matrix(&player_context->transform_context);

	glm::vec3 forward	= transform::get_forward(current_transform_matrix) * (float)forward_back_axis;
	glm::vec3 right		= transform::get_right(current_transform_matrix) * (float)left_right_axis;
	glm::vec3 up		= glm::vec3(0, 1, 0) * (float)up_down_axis;

	//if ()
	float angle_div = 0.0005f;

	if (player_context->world_context->game_context->mouse_toggle == 1)
	{
		glm::quat y_look_direction = glm::angleAxis(input_manager_context->mouse_x * angle_div, glm::vec3(0, 1, 0));
		glm::quat x_look_direction = glm::angleAxis(input_manager_context->mouse_y * angle_div, glm::vec3(1, 0, 0));

		player_context->transform_context.rotation = x_look_direction * y_look_direction;
	}

	glm::vec3 intended_movement_direction = (forward + right + up);

	if (glm::length(intended_movement_direction) > 0)
	{
		intended_movement_direction = glm::normalize(intended_movement_direction) * delta_time * 4.0f;
	}
	else
	{
		intended_movement_direction = glm::vec3(0);
	}

	player_context->current_velocity += (intended_movement_direction - player_context->current_velocity) / (10.0f / delta_time);

	player_context->transform_context.position += player_context->current_velocity;

	camera->transform_context.position = player_context->transform_context.position;
	camera->transform_context.rotation = player_context->transform_context.rotation;

	bool mouse_click = input_manager::get_key_pressed(input_manager_context, GLFW_MOUSE_BUTTON_1);

	if (mouse_click)
	{
		forward = glm::normalize(transform::get_forward(current_transform_matrix));

		int block_count = 10;

		for (int i = 0; i < block_count; ++i)
		{
			glm::vec3 test_position = player_context->transform_context.position + (forward * (float)i);

			glm::ivec3 i_position = test_position;


		}
	}
}

static void player_update_lab4(player* player_context)
{
	render_camera* camera = &player_context->world_context->main_camera;
	render_window* render_window = player_context->world_context->game_context->window_context;
	input_manager* input_manager_context = &player_context->world_context->game_context->input_manager_context;

	float delta_time = render_window->delta_time;

	int forward_back_axis = -input_manager::get_key_down(input_manager_context, GLFW_KEY_W) + input_manager::get_key_down(input_manager_context, GLFW_KEY_S);
	int left_right_axis = input_manager::get_key_down(input_manager_context, GLFW_KEY_D) - input_manager::get_key_down(input_manager_context, GLFW_KEY_A);
	int up_down_axis = input_manager::get_key_down(input_manager_context, GLFW_KEY_SPACE) - input_manager::get_key_down(input_manager_context, GLFW_KEY_LEFT_SHIFT);

	glm::mat4 current_transform_matrix = transform::get_local_transform_matrix(&player_context->transform_context);

	glm::vec3 forward = transform::get_forward(current_transform_matrix) * (float)forward_back_axis;
	glm::vec3 right = transform::get_right(current_transform_matrix) * (float)left_right_axis;
	glm::vec3 up = glm::vec3(0, 1, 0) * (float)up_down_axis;

	float angle_div = 0.0005f;

	if (player_context->world_context->game_context->mouse_toggle == 1)
	{
		glm::quat y_look_direction = glm::angleAxis(input_manager_context->mouse_x * angle_div, glm::vec3(0, 1, 0));
		glm::quat x_look_direction = glm::angleAxis(input_manager_context->mouse_y * angle_div, glm::vec3(1, 0, 0));

		player_context->transform_context.rotation = x_look_direction * y_look_direction;
	}

	glm::vec3 intended_movement_direction = (forward + right + up);

	if (glm::length(intended_movement_direction) > 0)
	{
		intended_movement_direction = glm::normalize(intended_movement_direction) * delta_time * 0.05f;

		intended_movement_direction = glm::vec3(intended_movement_direction.x, 0, intended_movement_direction.z);
	}
	else
	{
		intended_movement_direction = glm::vec3(0);
	}

	player_context->current_velocity += intended_movement_direction;

	player_context->current_velocity.x += (0 - player_context->current_velocity.x) / (10.0f / delta_time);
	player_context->current_velocity.z += (0 - player_context->current_velocity.z) / (10.0f / delta_time);

	if (abs(player_context->current_velocity.y) < 4)
	{
		player_context->current_velocity.y -= delta_time * 0.07f;
	}

	float bottom = 5;

	bool grounded = false;

	if ((player_context->transform_context.position.y + player_context->current_velocity.y) <= bottom && !(in_range(player_context->transform_context.position.x) && in_range(player_context->transform_context.position.z)))
	{
		player_context->transform_context.position.y = bottom;
		player_context->current_velocity.y = 0;

		grounded = true;
	}

	if (input_manager::get_key_pressed(input_manager_context, GLFW_KEY_SPACE) && grounded)
	{
		player_context->current_velocity.y = 2;
	}

	player_context->transform_context.position += player_context->current_velocity * delta_time;

	camera->transform_context.position = player_context->transform_context.position;
	camera->transform_context.rotation = player_context->transform_context.rotation;
}

void player::update(player* player_context)
{
	switch (player_context->mode)
	{
	case player_mode_spectator: player_update_spectator(player_context); break;
	case player_mode_lab4: player_update_lab4(player_context); break;
	default: assert(false); throw 0; assert(false); break;
	}
}
