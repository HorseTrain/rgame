#include "asset_collection.h"
#include <assert.h>

void asset_instance::create(asset_instance* result, asset_instantiation create, asset_instantiation destroy)
{
	result->buffer = nullptr;
	result->create_function = create;
	result->destroy_function = destroy;
}

void asset_instance::create_asset(asset_instance* asset_instance_context, void** arguments)
{
	if (asset_instance_context->create_function == nullptr)
		return;

	asset_instance_context->create_function(asset_instance_context,arguments);
}

void asset_instance::destroy_asset(asset_instance* asset_instance_context, void** arguments)
{
	if (asset_instance_context->destroy_function == nullptr)
		return;

	asset_instance_context->destroy_function(asset_instance_context, arguments);
}

void asset_collection::create(asset_collection* result)
{
	result->all_assets = std::unordered_set<asset_instance*>();
}

void asset_collection::destroy(asset_collection* to_destroy)
{
	for (auto i : to_destroy->all_assets)
	{
		destroy_asset(i);
	}
}

void asset_collection::create_asset_instance(asset_instance** result, asset_collection* collection, asset_instantiation create_function, asset_instantiation destroy_function, std::string name)
{
	*result = new asset_instance();

	asset_instance::create(*result, create_function, destroy_function);

	(*result)->name = name;
	(*result)->asset_collection_context = collection;

	if (name != "")
	{
		if (collection->asset_map.find(name) != collection->asset_map.end())
		{
			assert(false);

			throw 0;
		}

		collection->asset_map[name] = *result;
	}

	collection->all_assets.insert(*result);
}

void asset_collection::destroy_asset(asset_instance* asset_instance_context)
{
	if (asset_instance_context->name != "")
	{
		asset_instance_context->asset_collection_context->asset_map.erase(asset_instance_context->name);
	}

	asset_instance::destroy_asset(asset_instance_context, nullptr);

	delete asset_instance_context;
}