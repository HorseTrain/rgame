#include "texture_loader.h"

#include "asset/asset_collection.h"
#include "rgame/io.h"
#include "rgame/render_texture.h"

static void destroy_texture(asset_instance* asset_instance_context, void** arguments)
{
	render_texture* render_texture_context = (render_texture*)asset_instance_context->buffer;

	render_texture::destroy(render_texture_context);

	delete render_texture_context;
}

void load_texture(std::string texture_name, std::string source_path, asset_collection* asset_collection_context, io* io_context)
{
	source_path = io::get_path(io_context, source_path);
	
	render_texture* result = new render_texture();
	render_texture::load_from_file(result, source_path);

	asset_instance* asset_instance_context;
	asset_collection::create_asset_instance(&asset_instance_context, asset_collection_context, nullptr, destroy_texture, texture_name);
	
	asset_instance_context->buffer = result;
}