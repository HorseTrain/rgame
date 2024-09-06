#pragma once

#ifndef OBJ_MODEL_SCENE_H
#define OBJ_MODEL_SCENE_H

#include <string>
#include <vector>

#include "rgame/memory_tools.h"
#include "wavefront_mesh.h"

struct game_scene;

struct obj_model_scene
{
	arena_allocator							allocator;

	game_scene*								raw_scene;
	intrusive_linked_list<wavefront_mesh*>*	meshes;

	static void create(obj_model_scene* result, game_scene* raw_scene, std::string obj_path);
	static void destroy(obj_model_scene* to_destroy);
	static void update(obj_model_scene* scene);
	static void create_empty_wavefront_mesh_object(obj_model_scene* scene, wavefront_mesh** result);
	static void load_obj_from_file(obj_model_scene* scene, std::string path);
};

#endif