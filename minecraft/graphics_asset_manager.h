#pragma once

#ifndef GRAPHICS_ASSET_MANAGER_H
#define GRAPHICS_ASSET_MANAGER_H

#include <string>
#include <map>

struct graphics_asset_manager;

enum asset_type
{
	minecraft_shader,
	minecraft_texture,
};

struct graphics_asset
{
	std::string name;
	asset_type	type;
	void*		data;

	static void create(graphics_asset** graphics_asset_context, graphics_asset_manager* graphics_asset_manager_context, asset_type type, std::string name, void* data);
	static void destroy(graphics_asset* graphics_asset_context);
};

struct graphics_asset_manager
{
	std::map<std::string, graphics_asset*>	assets;

	static void								create(graphics_asset_manager* graphics_asset_manager_context);
	static void								destroy(graphics_asset_manager* to_destroy);
	static void								destroy_asset(graphics_asset_manager* graphics_asset_manager_context, std::string name);
	static bool								asset_exists(graphics_asset_manager* graphics_asset_manager_context, std::string name);
};

#endif