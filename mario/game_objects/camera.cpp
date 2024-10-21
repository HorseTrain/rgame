#include "camera.h"

#include "mario.h"

void camera::create(camera** result, level* level_context, render_camera* render_camera_context, glm::vec3 start_position, float perspective, float near, float far)
{
	game_object* working_object;

	game_object::create(&working_object, "", level_context);
	camera* camera_data = game_object::allocate_game_object_data<camera>(working_object);

	camera_data->render_camera_context = render_camera_context;

	render_camera::create_empty(render_camera_context, perspective, near, far);
	render_camera_context->transform_context.position = start_position;

	*result = camera_data;
}

void camera::start(camera* camera_context)
{
	camera_context->render_camera_context->transform_context.position = glm::vec3(0, 0, 10);
}

void camera::update(camera* camera_context)
{
}

void camera::destroy(camera* debug_camera_context)
{
	//
}

glm::mat4 camera::get_camera_perspective(camera* working_camera)
{
	float aspect = render_window::get_aspect_ratio(working_camera->game_object_context->game_context->window_context);

	return render_camera::get_view_matrix(working_camera->render_camera_context, aspect);
}
