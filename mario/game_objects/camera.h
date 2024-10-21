#pragma once

#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H

#include "rgame/glm/vec3.hpp"
#include "rgame/glm/mat4x4.hpp"

struct game_object;
struct render_camera;
struct level;
struct render_camera;

struct camera
{
	game_object*	game_object_context;
	render_camera*	render_camera_context;

	static void			create(camera** result,level* level_context, render_camera* render_camera_context, glm::vec3 start_position, float perspective, float near, float far);
	static glm::mat4	get_camera_perspective(camera* working_camera);

	static void		start(camera* camera_context);
	static void		update(camera* camera_context);
	static void		destroy(camera* debug_camera_context);
};

#endif