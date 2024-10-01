#include "render_model.h"
#include "asset/asset_instance.h"

void render_model::create(render_model* result, std::string name)
{
	result->name = name;
}

void render_model::destroy(render_model* to_destroy)
{
	for (auto i : to_destroy->meshes)
	{
		static_render_mesh::destroy(i.second);

		delete i.second;
	}
}

void render_model::draw(render_model* render_model_context)
{
	for (auto i : render_model_context->meshes)
	{
		static_render_mesh::draw(i.second);
	}
}

void render_model::create_static_render_mesh(render_model* render_model_context, static_render_mesh** result, std::string name)
{
	if (render_model_context->meshes.find(name) != render_model_context->meshes.end())
	{
		assert(false);

		throw 0;
	}

	*result = new static_render_mesh();
	render_model_context->meshes[name] = *result;
}

static void create_render_model_with_asset_collection(asset_instance* instance, void** arguments)
{
	render_model** result = (render_model**)arguments[0];

	*result = new render_model();
	instance->buffer = *result;
}

static void destroy_render_model_with_asset_collection(asset_instance* instance, void** arguments)
{
	render_model* working_model = (render_model*)instance->buffer;

	render_model::destroy(working_model);

	delete working_model;
}

void render_model::create_with_asset_collection(asset_collection* collection_context, render_model** result, std::string name)
{
	asset_instance* instance;

	asset_collection::create_asset_instance(&instance, collection_context);

	asset_instance::create(instance, create_render_model_with_asset_collection, destroy_render_model_with_asset_collection);

	void* arguments[] = {
		result
	};

	asset_instance::create_asset(instance, arguments);
}
