#include "player.h"
#include "rgame/render_camera.h"
#include "rgame/transform.h"
#include "rgame/render_window.h"
#include "game.h"
#include "world.h"

#include "chunk.h"
#include "block.h"

void player::create(player* player_context, world* world_context)
{
	player_context->world_context = world_context;

	player_context->mode = player_mode_spectator;

	transform::create_identity(&player_context->transform_context);

	player_context->transform_context.position = glm::vec3(20.2, 100, 30.1);
}

void player::destroy(player* player_context)
{
	//
}

static void draw_debug_retical(render_window* render_window_context)
{
	glUseProgram(0);

	glBegin(GL_QUADS);

	float size = 0.01f;

	float width = size / render_window::get_aspect_ratio(render_window_context);
	float height = size;

	glVertex3f(width, height, 0);
	glVertex3f(-width, height, 0);
	glVertex3f(-width, -height, 0);
	glVertex3f(width, -height, 0);

	glEnd();
}

static glm::quat rotate_camera(player* player_context, input_manager* input_manager_context, render_camera* camera)
{
	//Camera Rotation
	float angle_div = 0.0005f;

	if (player_context->world_context->game_context->mouse_toggle == 1)
	{
		player_context->mouse += glm::vec2(input_manager_context->mouse_velocity_x, input_manager_context->mouse_velocity_y);
	}

	glm::quat y_look_direction = glm::angleAxis(player_context->mouse.x * angle_div, glm::vec3(0, 1, 0));
	glm::quat x_look_direction = glm::angleAxis(player_context->mouse.y * angle_div, glm::vec3(1, 0, 0));

	player_context->transform_context.rotation = x_look_direction * y_look_direction;

	camera->transform_context.position = player_context->transform_context.position + glm::vec3(0, 1.9f, 0);
	camera->transform_context.rotation = player_context->transform_context.rotation;

	return y_look_direction;
}

struct block_get
{
	block*		block_context;
	glm::ivec3	block_position;
};

static std::vector<block_get> get_block(world* world_context, glm::vec3 start, glm::vec3 end)
{
	glm::vec3 difference = (end - start);

	int count = glm::length(difference);

	count++;

	difference = glm::normalize(difference);

	std::vector<block_get> working_results;

	for (int i = 0; i < count; ++i)
	{
		glm::vec3 working_position = start + (difference * (float)i);

		chunk* working_chunk;
		block* working_block;

		world::get_block(world_context, working_position, &working_block, &working_chunk);

		working_results.push_back({working_block, working_position});
	}

	return working_results;
}

static block_get get_collidable_block(std::vector<block_get>* blocks)
{
	for (int i = 0; i < blocks->size(); ++i)
	{
		if (!blocks->data()[i].block_context->is_transparent)
			return blocks->data()[i];
	}

	return { nullptr, glm::vec3(0,0,0) };
}

static void break_place_blocks(player* player_context, render_camera* render_camera_context,input_manager* input_manager_context, render_window* render_window_context)
{
	bool mouse_click = input_manager::get_key_pressed(input_manager_context, GLFW_MOUSE_BUTTON_1);

	player_context->forward = glm::normalize(transform::get_forward(glm::mat4_cast(player_context->transform_context.rotation)));;

	if (mouse_click)
	{
		glm::vec3 camera_forward = player_context->forward;

		int block_count = 1000;

		for (int i = 1; i < block_count; ++i)
		{
			glm::vec3 test_position = render_camera_context->transform_context.position + (-camera_forward * (float)i);

			glm::ivec3 i_position = test_position;

			chunk* working_chunk;
			block* working_block;

			world::get_block(player_context->world_context, i_position, &working_block, &working_chunk);

			if (!working_block->is_transparent)
			{
				working_block->is_transparent = true;
				chunk::regenerate_mesh(working_chunk, true);

				break;
			}
		}
	}

	draw_debug_retical(render_window_context);
}

static bool in_range(float start, float check, float size)
{
	return start >= check && start <= (check + size);
}

static bool in_box(glm::vec2 start, glm::vec2 point, float size)
{
	return in_range(start.x, point.x, size) && in_range(start.y, point.y, size);
}

static glm::vec3 get_collision_point(glm::vec3 block_location, glm::ivec3 check_location, int side, bool* has_collision)
{
	switch (side)
	{
	case NEIGHBOR_PY:
	{
		if (check_location.y >= block_location.y)
		{
			if (in_box(glm::vec2(block_location.x, block_location.z), glm::vec2(check_location.x, check_location.z), 1))
			{
				*has_collision = true;

				return block_location + glm::vec3(check_location.x, 1, check_location.z);
			}
		}
	}; break;

	case NEIGHBOR_NZ:
	{
		if (check_location.z >= block_location.z)
		{
			if (in_box(glm::vec2(block_location.x, block_location.y), glm::vec2(check_location.x, check_location.y), 1))
			{
				*has_collision = true;

				return block_location + glm::vec3(check_location.x, check_location.y, 0);
			}
		}
	}; break;

	default: throw 0;
	}

	*has_collision = false;
}

static float clamp_float(float source)
{
	if (source < 0)
		return source;

	return source;
}

static void lerp_to(float* source, float des, float lerp)
{
	*source += (des - *source) / lerp;
}

static void append_one_to_direction(float* source)
{
	if (*source < 0)
		*source -= 1;
	else
		*source += 1;
}

static void player_update_physics(player* player_context, glm::quat looking_direction, input_manager* input_manager_context, render_window* render_window_context)
{
	int forward_back_axis = -input_manager::get_key_down(input_manager_context, GLFW_KEY_W) + input_manager::get_key_down(input_manager_context, GLFW_KEY_S);
	int left_right_axis = input_manager::get_key_down(input_manager_context, GLFW_KEY_D) - input_manager::get_key_down(input_manager_context, GLFW_KEY_A);
	int up_down_axis = input_manager::get_key_down(input_manager_context, GLFW_KEY_SPACE) - input_manager::get_key_down(input_manager_context, GLFW_KEY_LEFT_SHIFT);

	glm::mat4 transform = glm::mat4_cast(looking_direction);

	glm::vec3 player_position = player_context->transform_context.position;
	glm::vec3 directional_forward = transform::get_forward(glm::mat4_cast(looking_direction));
	glm::vec3 directional_right = transform::get_right(glm::mat4_cast(looking_direction));

	float delta_time = render_window_context->delta_time;

	float& x_vel = player_context->current_velocity.x;
	float& y_vel = player_context->current_velocity.y;
	float& z_vel = player_context->current_velocity.z;

	float gravity = delta_time * 0.01f;

	y_vel -= gravity;

	float speed = 0.01f;

	player_context->current_velocity += (directional_forward * (float)forward_back_axis * delta_time * speed) + (directional_right * (float)left_right_axis * delta_time * speed);
	lerp_to(&x_vel, 0, 10 / delta_time);
	lerp_to(&z_vel, 0, 10 / delta_time);

	for (int i = 0; i < 2; ++i)
	{
		float z_offset = z_vel;
		float x_offset = x_vel;

		append_one_to_direction(&z_offset);
		append_one_to_direction(&x_offset);

		std::vector<block_get> x_block_ray = get_block(player_context->world_context, player_position + glm::vec3(0, 0.5f + i, 0), player_position + glm::vec3(x_offset, 0.5f + i, 0));
		std::vector<block_get> z_block_ray = get_block(player_context->world_context, player_position + glm::vec3(0, 0.5f + i, 0), player_position + glm::vec3(0, 0.5f + i, z_offset));

		block_get working_z_block = get_collidable_block(&z_block_ray);
		block_get working_x_block = get_collidable_block(&x_block_ray);

		if (working_z_block.block_context != nullptr)
		{
			z_vel = 0;
		}

		if (working_x_block.block_context != nullptr)
		{
			x_vel = 0;
		}
	}

	bool grounded = false;

	{
		std::vector<block_get> downward_block_ray = get_block(player_context->world_context, player_position + glm::vec3(0, 1, 0), player_position + glm::vec3(0, y_vel - 1, 0));

		block_get working_block = get_collidable_block(&downward_block_ray);

		if (working_block.block_context != nullptr)
		{
			bool has_collision;

			glm::vec3 downward_collision_point = get_collision_point(working_block.block_position, player_position, NEIGHBOR_PY, &has_collision);

			if (has_collision && player_position.y <= downward_collision_point.y - y_vel)
			{
				player_context->transform_context.position.y = downward_collision_point.y;
				y_vel = 0;

				grounded = true;
			}
		}
	}

	if (input_manager::get_key_pressed(input_manager_context, GLFW_KEY_SPACE) && grounded)
	{
		y_vel = 0.2f;
	}

	player_context->transform_context.position += player_context->current_velocity * delta_time;
}

static void player_update_spectator(player* player_context)
{
	render_camera* camera = &player_context->world_context->main_camera;
	render_window* render_window_context = player_context->world_context->game_context->window_context;
	input_manager* input_manager_context = &player_context->world_context->game_context->input_manager_context;

	float delta_time = render_window_context->delta_time;

	glm::quat looking_direction = rotate_camera(player_context, input_manager_context, camera);

	player_update_physics(player_context, looking_direction, input_manager_context, render_window_context);

	break_place_blocks(player_context,camera ,input_manager_context, render_window_context);
}

void player::update(player* player_context)
{
	switch (player_context->mode)
	{
	case player_mode_spectator: player_update_spectator(player_context); break;
	default: assert(false); throw 0; assert(false); break; 
	}
}
