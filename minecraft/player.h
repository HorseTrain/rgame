#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "rgame/transform.h"

struct world;
struct render_window;

struct control_surface
{
	int input_fb;
	int input_lr;
	int input_ud;

	bool mine;
};

struct player
{
	world*			world_context;
	control_surface control_data;
	transform		transform_context;

	static void create(player* result, world* world_context);
	static void destroy(player* to_destroy);
	static void update(player* player_context);
	static void player_update_controles(player* player_context);
	static void player_move_debug(player* player_context);
	static render_window* get_render_window(player* player_context);
	static void player_update_camera(player* player_context);
};

#endif