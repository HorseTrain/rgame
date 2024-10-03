#include "debug_fp_player.h"
#include "level/level_scene.h"
#include "rgame/render_window.h"
#include "game/game_context.h"
#include "actor.h"
#include "actor_ids.h"
#include "floor_actor.h"
#include "rgame/r_math.h"

#define PLAYER_HEIGHT 1

int lock_mode = 1;

static void swap(float* left, float* right)
{
	float tmp = *left;

	*left = *right;
	*right = tmp;
}

static bool in_range(float left, float right, float point)
{
	if (left > right)
	{
		swap(&left, &right);
	}

	return point >= left && point <= right;
}

static bool in_range(glm::vec2 top_left, glm::vec2 bottom_right, glm::vec2 point)
{
	bool x_valid = in_range(top_left.x, bottom_right.x, point.x);
	bool y_valid = in_range(top_left.y, bottom_right.y, point.y);

	return x_valid && y_valid;
}

void update_player(actor* level_actor)
{
	level_scene* scene = level_actor->level_scene_context;
	render_camera* camera = &scene->main_camera;
	render_window* window = scene->game_context_context->render_window_context;
	debug_fp_player_actor* player = (debug_fp_player_actor*)level_actor->actor_data;
	input_manager* input = &scene->game_context_context->input;

	player->velocity += glm::vec3(0, -0.005f, 0) * window->delta_time;

	player->is_grounded = false;

	for (auto i : scene->actors)
	{
		if (i->actor_id != actor_id::actor_id_floor)
			continue;

		floor_actor* floor = (floor_actor*)i->actor_data;

		if (player->player_transform.position.y < floor->floor_transform.position.y)
		{
			continue;
		}

		glm::vec2 top_left = glm::vec2(-floor->size / 2, floor->size / 2);
		glm::vec2 bottom_right = glm::vec2(floor->size / 2, -floor->size / 2);

		top_left += glm::vec2(floor->floor_transform.position.x, floor->floor_transform.position.z);
		bottom_right += glm::vec2(floor->floor_transform.position.x, floor->floor_transform.position.z);

		if (in_range(top_left, bottom_right, glm::vec2(player->player_transform.position.x, player->player_transform.position.z)))
		{
			if ((player->player_transform.position.y <= floor->floor_transform.position.y + PLAYER_HEIGHT) && (player->velocity.y <= 0))
			{
				player->player_transform.position.y = floor->floor_transform.position.y + PLAYER_HEIGHT;
				player->velocity.y = 0;

				player->is_grounded = true;

				player->player_transform.position += floor->velocity;
			}
		}
	}

	if (player->player_transform.position.y < PLAYER_HEIGHT)
	{

	}

	if (input_manager::get_key_pressed(input, GLFW_KEY_ESCAPE))
	{
		lock_mode *= -1;

		if (lock_mode == -1)
		{
			input_manager::lock_mouse(input);
		}
		else
		{
			input_manager::unlock_mouse(input);
		}
	}

	if (lock_mode == -1)
	{
		float sensitivity = 0.0008f;

		float difference = input->mouse_y - player->last_mouse_y;

		player->current_mouse_y += difference * sensitivity;

		float angle_clamp = M_PI / 4;

		if (player->current_mouse_y < -angle_clamp)
		{
			player->current_mouse_y = -angle_clamp;
		}

		if (player->current_mouse_y > angle_clamp)
		{
			player->current_mouse_y = angle_clamp;
		}

		glm::quat x_rotation = glm::angleAxis((float)input->mouse_x * sensitivity, glm::vec3(0, 1, 0));
		glm::quat y_rotation = glm::angleAxis(player->current_mouse_y, glm::vec3(1, 0, 0));

		player->player_transform.rotation = y_rotation * x_rotation;
	}

	player->last_mouse_y = input->mouse_y;

	float x_movement = input_manager::get_key_down(input, GLFW_KEY_D) - input_manager::get_key_down(input, GLFW_KEY_A);
	float z_movement = input_manager::get_key_down(input, GLFW_KEY_W) - input_manager::get_key_down(input, GLFW_KEY_S);

	glm::mat4 transform_matrix = transform::get_local_transform_matrix(&player->player_transform);

	glm::vec3 target_velocity = glm::vec3(0);

	target_velocity += transform::get_forward(transform_matrix) * -z_movement * window->delta_time * 0.01f;
	target_velocity += transform::get_right(transform_matrix) * x_movement * window->delta_time * 0.01f;

	target_velocity.y = 0;

	player->velocity += target_velocity;

	float friction = 20;

	player->velocity.x += (0 - player->velocity.x) / (friction / window->delta_time);
	player->velocity.z += (0 - player->velocity.z) / (friction / window->delta_time);
		
	if (player->is_grounded && input_manager::get_key_down(input, GLFW_KEY_SPACE))
	{
		player->velocity.y = 0.2f;
	}

	player->player_transform.position += player->velocity * window->delta_time;

	camera->transform_context.position = player->player_transform.position;
	camera->transform_context.rotation = player->player_transform.rotation;
}

void destroy_player(actor* level_actor)
{
	debug_fp_player_actor* debug_fp_player_actor_data = (debug_fp_player_actor*)level_actor->actor_data;

	delete debug_fp_player_actor_data;
}

void debug_fp_player_actor::create(level_scene* scene)
{
	debug_fp_player_actor* debug_fp_player_actor_data = new debug_fp_player_actor();

	actor* level_actor = level_scene::create_empty_actor(scene);
	level_actor->actor_data = debug_fp_player_actor_data;

	level_actor->actor_id = actor_id::actor_id_debug_player;

	debug_fp_player_actor_data->actor_context = level_actor;

	transform::create_identity(&debug_fp_player_actor_data->player_transform);

	level_actor->actor_update = update_player;
	level_actor->actor_destroy = destroy_player;

	debug_fp_player_actor_data->player_transform.position = glm::vec3(0, 10, 5);
}
