#pragma once

#ifndef FLOOR_ACTOR_H
#define FLOOR_ACTOR_H

#include "rgame/transform.h"
#include "render/static_render_mesh.h"

struct actor;
struct level_scene;

struct floor_actor
{
	transform			floor_transform;
	actor*				actor_context;

	static_render_mesh	floor_mesh;
	float				size;

	bool				move;

	float				time;
	glm::vec3			velocity;

	static floor_actor* create(level_scene* scene, float size);
};

#endif