#pragma once

#ifndef ASSET_INSTANCE_H
#define ASSET_INSTANCE_H

#include <unordered_set>
#include <map>
#include <string>

struct asset_instance;
struct asset_collection;

typedef void(*asset_instantiation)(asset_instance*, void** arguments);

struct asset_instance
{
	void*					buffer;
	asset_instantiation		create_function;
	asset_instantiation		destroy_function;

	asset_collection*		asset_collection_context;

	std::string				name;

	static void				create(asset_instance* result, asset_instantiation create, asset_instantiation destroy);
	static void				create_asset(asset_instance* asset_instance_context, void** arguments);
	static void				destroy_asset(asset_instance* asset_instance_context, void** arguments);
};

struct asset_collection
{
	std::unordered_set<asset_instance*>		all_assets;
	std::map<std::string, asset_instance*>	asset_map;

	static void								create(asset_collection* result);
	static void								destroy(asset_collection* to_destroy);
	static void								create_asset_instance(asset_instance** result, asset_collection* collection, asset_instantiation create = nullptr, asset_instantiation destroy = nullptr,  std::string name = "");
	static void								destroy_asset(asset_instance* asset_instance_context);

	template <typename T>
	static T* get_asset_by_name(asset_collection* collection, std::string name)
	{
		if (collection->asset_map.find(name) == collection->asset_map.end())
			return nullptr;

		return (T*)collection->asset_map[name]->buffer;
	}
};

#endif