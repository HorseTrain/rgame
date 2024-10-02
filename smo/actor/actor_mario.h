#pragma once

#ifndef MARIO_H
#define MARIO_H

#include "rgame/transform.h"

struct render_model;
struct asset_collection;
struct io;
struct actor;
struct level_scene;
struct game_context;

struct actor_mario
{
	actor*			actor_reference;
	level_scene*	level_scene_context;
	game_context*	game_context_context;

	transform		actor_transform;
	transform		model_transform;

	transform		camera_pivot;
	transform		camera_transform;

	render_model*	mario_body;
	render_model*	mario_head;
	render_model*	mario_hand_l;
	render_model*	mario_hand_r;

	float			camera_distance_to_player;

	static void		create_actor(level_scene* level_scene_context);

	static void		create(actor_mario* to_create);
	static void		update(actor_mario* to_update);
	static void		destroy(actor_mario* to_destroy);
};

#endif