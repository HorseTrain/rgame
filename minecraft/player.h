#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "rgame/transform.h"
#include "rgame/glm/vec3.hpp"
#include "rgame/glm/vec2.hpp"

struct world;

enum player_mode
{
	player_mode_spectator,
	player_mode_lab4
};

struct player
{
	world*		world_context;
	transform	transform_context;
	player_mode mode;

	glm::vec3	forward;
	glm::ivec2	mouse;

	glm::vec3	current_velocity;

	static void create(player* player_context, world* game_context);
	static void destroy(player* player_context);
	static void update(player* player_context);
};

#endif