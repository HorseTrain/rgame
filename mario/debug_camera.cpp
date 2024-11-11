#include "debug_camera.h"
#include "level.h"
#include "game_object.h"

#include "rgame/lifetime_memory_manager.h"
#include "rgame/render_camera.h"
#include "mario/game.h"
#include "rgame/input_manager.h"
#include "rgame/r_math.h"

static void clamp_angle(glm::vec2* working)
{
	clamp_angle(&working->x);
	clamp_angle(&working->y);
}

void debug_camera::create(debug_camera** output, level* level_context)
{
	debug_camera* result = game_object::create_game_object<debug_camera>(level_context, "");

	result->working_render_camera = &level_context->working_camera;
	result->transform_context = &result->working_render_camera->transform_context;
	result->input = result->game_object_context->game_context->input_manager_context;

	result->sensitivity = 0.001;

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

	glm::ivec2 mouse_position = { input->mouse_x, input->mouse_y };

	glm::ivec2 mouse_velocity = mouse_position - debug_camera_context->last_mouse_position;

	debug_camera_context->view_rotation += glm::vec2(mouse_velocity.y, mouse_velocity.x) * debug_camera_context->sensitivity;
	clamp_angle(&debug_camera_context->view_rotation);

	debug_camera_context->last_mouse_position = mouse_position;
}

void debug_camera::destroy(debug_camera* debug_camera_context)
{

}
