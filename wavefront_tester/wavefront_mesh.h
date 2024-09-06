#pragma once

#ifndef WAVEFRONT_MESH_H
#define WAVEFRONT_MESH_H

#include "rgame/render_mesh.h"
#include "rgame/game_object.h"

struct obj_model_scene;
struct wavefront_mesh_gameobject_data;

struct wavefront_mesh
{
	std::string						name;
	render_mesh						mesh;
	obj_model_scene*				scene;
	game_object						_game_object;
	wavefront_mesh_gameobject_data* game_object_data;
};

#endif