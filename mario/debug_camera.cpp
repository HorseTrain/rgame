#include "debug_camera.h"
#include "level.h"
#include "game_object.h"

#include "rgame/lifetime_memory_manager.h"
#include "rgame/render_camera.h"
#include "mario/game.h"
#include "rgame/input_manager.h"
#include "rgame/r_math.h"

static void mod_angle(glm::vec2* working)
{
	mod_angle(&working->x);
	mod_angle(&working->y);
}

void debug_camera::create(debug_camera** output, level* level_context)
{
	debug_camera* result = game_object::create_game_object<debug_camera>(level_context, "");

	result->working_render_camera = &level_context->working_camera;
	result->transform_context = &result->working_render_camera->transform_context;
	result->input = result->game_object_context->game_context->input_manager_context;

	result->sensitivity = 0.001;
	result->speed = 0.1f;

	render_camera::create_empty(&level_context->working_camera, 60, 1, 1000);

	*output = result;
}

void debug_camera::start(debug_camera* debug_camera_context)
{
	debug_camera_context->transform_context->position = glm::vec3(0, 0, 10);
	debug_camera_context->camera_is_active = false;
}

void debug_camera::update(debug_camera* debug_camera_context)
{
	input_manager* input = debug_camera_context->input;
	render_window* render_window_context = debug_camera_context->game_object_context->game_context->window_context;
	transform* transform_context = debug_camera_context->transform_context;
	glm::vec2* view_rotation = &debug_camera_context->view_rotation;
	glm::vec3* velocity_vector = &debug_camera_context->velocity_vector;

	if (input_manager::get_key_pressed(input, GLFW_KEY_ESCAPE))
	{
		if (debug_camera_context->camera_is_active)
		{
			debug_camera_context->camera_is_active = false;
		}
		else
		{
			debug_camera_context->camera_is_active = true;
		}

		if (debug_camera_context->camera_is_active)
		{
			input_manager::lock_mouse(input);
		}
		else
		{
			input_manager::unlock_mouse(input);
		}
	}

	if (input_manager::get_key_pressed(input, GLFW_KEY_R))
	{
		transform_context->position = { 0, 0, 10 };
		debug_camera_context->view_rotation = { 0,0 };
	}

	glm::ivec2 mouse_position = { input->mouse_x, input->mouse_y };

	glm::ivec2 mouse_velocity = mouse_position - debug_camera_context->last_mouse_position;

	if (debug_camera_context->camera_is_active)
	{
		*view_rotation += glm::vec2(mouse_velocity.y, mouse_velocity.x) * debug_camera_context->sensitivity;
	}

	clamp_angle(&view_rotation->x, -M_PI / 2, M_PI / 2);

	debug_camera_context->transform_context->rotation = glm::angleAxis(view_rotation->x, glm::vec3(1, 0, 0)) * glm::angleAxis(view_rotation->y, glm::vec3(0, 1, 0));

	glm::mat4 view_matrix = transform::get_local_camera_transform_matrix(debug_camera_context->transform_context);

	glm::vec3 forward = transform::get_forward(view_matrix);
	glm::vec3 right = transform::get_right(view_matrix);
	glm::vec3 up = glm::vec3(0, 1, 0);

	forward.y = 0;
	right.y = 0;

	forward = glm::normalize(forward);
	right = glm::normalize(right);

	glm::vec3 target_velocity_vector = glm::vec3(
		-input_manager::get_key_down(input, GLFW_KEY_A) + input_manager::get_key_down(input, GLFW_KEY_D),
		-input_manager::get_key_down(input, GLFW_KEY_LEFT_SHIFT) + input_manager::get_key_down(input, GLFW_KEY_SPACE),
		input_manager::get_key_down(input, GLFW_KEY_S) - input_manager::get_key_down(input, GLFW_KEY_W)
	);

	if (glm::length(target_velocity_vector) > 0)
	{
		target_velocity_vector = glm::normalize(target_velocity_vector) * debug_camera_context->speed;
	}

	*velocity_vector += (target_velocity_vector - *velocity_vector) / (10.0f / render_window_context->delta_time);

	glm::vec3 target_velocity = (forward * velocity_vector->z) + (right * velocity_vector->x) + (up * velocity_vector->y);

	transform_context->position += target_velocity;

	debug_camera_context->last_mouse_position = mouse_position;
}

void debug_camera::destroy(debug_camera* debug_camera_context)
{

}
