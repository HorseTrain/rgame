#include "render_texture_manager.h"
#include "rgame/render_texture.h"
#include "graphics_asset_manager.h"

void create_render_texture(render_texture** result, graphics_asset_manager* manager, std::string name)
{
	render_texture* working_result;

	working_result = new render_texture();
	render_texture::create(working_result);

	graphics_asset* asset_to_append;
	graphics_asset::create(&asset_to_append, manager, minecraft_texture, name, working_result);

	*result = working_result;
}
