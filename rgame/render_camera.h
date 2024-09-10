#pragma once

#ifndef RENDER_CAMERA
#define RENDER_CAMERA

#include "transform.h"
#include "glm/mat4x4.hpp"

struct render_camera
{
	transform	transform_context;
	float		feild_of_view;
	float		z_near;
	float		z_far;

	static		void create_empty(render_camera* result, float feild_of_view, float z_near, float z_far);
	static		glm::mat4 get_view_matrix(render_camera* camera, float window_aspect);
};

#endif