#include "game_object.h"
#include "game_scene.h"

void game_scene::create(game_scene* result, render_window* window)
{
	result->window = window;
	result->objects = std::vector<game_object*>();

	render_camera::create_empty(&result->main_camera, 60, 1, 100);
}

void game_scene::scene_update(game_scene* scene)
{
	for (int i = 0; i < scene->objects.size(); ++i)
	{
		game_object* object = scene->objects[i];

		if (!object->started && object->object_start != nullptr)
		{
			object->object_start(object, scene);
			object->started = true;
		}

		if (object->object_update != nullptr)
		{
			object->object_update(object, scene);
		}
	}

	for (int i = 0; i < scene->objects.size(); ++i)
	{
		game_object* object = scene->objects[i];

		if (object->object_draw != nullptr)
		{
			object->object_draw(object, scene);
		}
	}
}
