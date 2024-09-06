#pragma once

#ifndef WORLD_GRAPHICS_MANAGER_H
#define WORLD_GRAPHICS_MANAGER_H

#include <map>
#include <rgame/render_shader.h>

struct world;

struct world_graphics_manager
{
	world*									world_context;
	std::map<std::string, render_shader*>	loaded_shaders;

	static void create(world_graphics_manager* result, world* world_context);
	static void destroy(world_graphics_manager* world_graphics_manager_context);
	static void load_shader(world_graphics_manager* world_graphics_manager_context, std::string name, std::string directory_path);
	static render_shader* create_render_shader(world_graphics_manager* world_graphics_manager_context, std::string name);
};

#endif