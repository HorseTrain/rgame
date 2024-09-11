#include "graphics_asset_manager.h"
#include <assert.h>
#include "rgame/render_shader.h"
#include "rgame/render_texture.h"

static void destroy_graphics_asset(graphics_asset* graphics_asset_context)
{
	graphics_asset::destroy(graphics_asset_context);

	delete graphics_asset_context;
}

void graphics_asset::create(graphics_asset** graphics_asset_context, graphics_asset_manager* graphics_asset_manager_context, asset_type type, std::string name, void* data)
{
	graphics_asset* working_result = new graphics_asset();

	working_result->type = type;
	working_result->name = name;
	working_result->data = data;

	if (graphics_asset_manager::asset_exists(graphics_asset_manager_context, name))
	{
		assert(false);

		throw 0;
	}

	graphics_asset_manager_context->assets[name] = working_result;

	*graphics_asset_context = working_result;
}

void graphics_asset::destroy(graphics_asset* graphics_asset_context)
{
	if (graphics_asset_context->data == nullptr)
		return;

	switch (graphics_asset_context->type)
	{
	case minecraft_texture:
	{
		render_texture::destroy((render_texture*)graphics_asset_context->data);
		delete (render_texture*)graphics_asset_context->data;
	}; break;

	case minecraft_shader:
	{
		render_shader::destroy((render_shader*)graphics_asset_context->data);
		delete (render_shader*)graphics_asset_context->data;
	}; break;

	default: 
		assert(false);
		throw 0;
	}
}

void graphics_asset_manager::create(graphics_asset_manager* graphics_asset_manager_context)
{
	graphics_asset_manager_context->assets = std::map<std::string, graphics_asset*>();
}

void graphics_asset_manager::destroy(graphics_asset_manager* to_destroy)
{
	for (auto i = to_destroy->assets.begin(); i != to_destroy->assets.end(); ++i)
	{
		graphics_asset* graphics_asset_context = i->second;

		destroy_graphics_asset(graphics_asset_context);
	}
}

void graphics_asset_manager::destroy_asset(graphics_asset_manager* graphics_asset_manager_context, std::string name)
{
	if (graphics_asset_manager_context->assets.find(name) == graphics_asset_manager_context->assets.end())
		return;

	graphics_asset* graphics_asset_context = graphics_asset_manager_context->assets[name];

	destroy_graphics_asset(graphics_asset_context);
}

bool graphics_asset_manager::asset_exists(graphics_asset_manager* graphics_asset_manager_context, std::string name)
{
	return graphics_asset_manager_context->assets.find(name) != graphics_asset_manager_context->assets.end();
}
