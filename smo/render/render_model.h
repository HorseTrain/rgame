#pragma once

#ifndef RENDER_MODEL
#define RENDER_MODEL

#include <map>
#include <string>
#include "static_render_mesh.h"

struct asset_collection;
struct asset_instance;

struct render_model
{
	std::string									name;
	std::map<std::string, static_render_mesh*>	meshes;

	static void									create(render_model* result, std::string name);
	static void									destroy(render_model* to_destroy);
	static void									draw(render_model* render_model_context);
	static void									create_static_render_mesh(render_model* render_model_context, static_render_mesh** result, std::string name);
	static void									create_with_asset_collection(asset_collection* collection_context, render_model** result, std::string name);
};

#endif