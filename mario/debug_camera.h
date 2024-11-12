#pragma once

#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H

#include "rgame/glm/vec2.hpp"
#include "rgame/glm/vec3.hpp"

struct game_object;
struct level;
struct render_camera;
struct input_manager;
struct transform;

struct debug_camera
{
	game_object* game_object_context;
	render_camera* working_render_camera;
	input_manager* input;
	transform* transform_context;

	float			sensitivity;
	float			speed;

	glm::vec2		view_rotation;
	glm::vec3		velocity_vector;

	glm::ivec2		last_mouse_position;

	bool			camera_is_active;

	static void create(debug_camera** result, level* level_context);

	static void start(debug_camera* debug_camera_context);
	static void update(debug_camera* debug_camera_context);
	static void destroy(debug_camera* debug_camera_context);
};

#endif