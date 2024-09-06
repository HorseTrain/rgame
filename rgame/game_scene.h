#pragma once

#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <vector>

#include "render_camera.h"

struct game_object;
struct render_window;

struct game_scene
{
	render_window*						window;

	render_camera						main_camera;
	std::vector<game_object*>			objects;

	static void							create(game_scene* result, render_window* window);
	static void							scene_update(game_scene* scene);
};

#endif