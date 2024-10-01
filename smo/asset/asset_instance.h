#pragma once

#ifndef ASSET_INSTANCE_H
#define ASSET_INSTANCE_H

#include <unordered_set>

struct asset_instance;

typedef void(*asset_instantiation)(asset_instance*, void** arguments);

struct asset_instance
{
	void*					buffer;
	asset_instantiation		create_function;
	asset_instantiation		destroy_function;

	static void				create(asset_instance* result, asset_instantiation create, asset_instantiation destroy);
	static void				create_asset(asset_instance* asset_instance_context, void** arguments);
	static void				destroy_asset(asset_instance* asset_instance_context, void** arguments);
};

struct asset_collection
{
	std::unordered_set<asset_instance*> all_assets;

	static void							create(asset_collection* result);
	static void							destroy(asset_collection* to_destroy);
	static void							create_asset_instance(asset_instance** result, asset_collection* collection);
	static void							destroy_asset(asset_instance* asset_instance_context);
};

#endif