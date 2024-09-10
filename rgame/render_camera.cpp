#include "render_camera.h"
#include "r_math.h"

void render_camera::create_empty(render_camera* result, float feild_of_view, float z_near, float z_far)
{
	result->feild_of_view = feild_of_view;
	result->z_near = z_near;
	result->z_far = z_far;

	transform::create_identity(&result->transform_context);
}

glm::mat4 render_camera::get_view_matrix(render_camera* camera, float window_aspect)
{
	if (std::isnan(window_aspect))
		return glm::mat4(1);

	glm::mat4 transform_matrix = transform::get_local_camera_transform_matrix(&camera->transform_context);

	return glm::perspective(degrees_to_radians(camera->feild_of_view), window_aspect, camera->z_near, camera->z_far) * transform_matrix;
}
