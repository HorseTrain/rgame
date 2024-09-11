#include "render_shader_manager.h"
#include "rgame/render_shader.h"
#include "graphics_asset_manager.h"
#include "assert.h"

void create_render_shader(render_shader** result, graphics_asset_manager* manager, std::string name)
{
	render_shader* working_result;

	working_result = new render_shader();
	render_shader::create(working_result);

	graphics_asset* asset_to_append;
	graphics_asset::create(&asset_to_append, manager,minecraft_shader, name, working_result);

	*result = working_result;
}
