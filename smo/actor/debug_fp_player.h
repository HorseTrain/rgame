#pragma once

#ifndef DEBUG_FP_PLAYER_ACTOR_H
#define DEBUG_FP_PLAYER_ACTOR_H

#include "rgame/transform.h"
#include "render/static_render_mesh.h"

struct actor;
struct level_scene;

struct debug_fp_player_actor
{
	transform	player_transform;
	actor*		actor_context;
	bool		is_grounded;

	float		last_mouse_y;
	float		current_mouse_y;

	glm::vec3	velocity;

	static void create(level_scene* scene);
};

#endif