#pragma once

#ifndef SPINNING_MARIO_ACTOR_H
#define SPINNING_MARIO_ACTOR_H

#include "rgame/transform.h"
#include "render/render_model.h"

struct actor;
struct level_scene;

struct spinning_mario_actor
{
	transform			_transform;
	actor*				actor_context;

	render_model*		model;

	float				mario_angle;

	static spinning_mario_actor* create(level_scene* scene);
};

#endif