#ifndef ACTOR_PROJECT_ONE_H
#define ACTOR_PROJECT_ONE_H

#include "rgame/transform.h"

struct actor;
struct level_scene;

struct actor_project_one
{
	actor*			actor_reference;
	transform		actor_transform;

	static void		create_actor(level_scene* level_scene_context);

	static void		create(actor_project_one* to_create);
	static void		update(actor_project_one* to_update);
	static void		destroy(actor_project_one* to_destroy);
};

#endif