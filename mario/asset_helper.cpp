#include "asset_helper.h"

#include "rgame/lifetime_memory_manager.h"
#include "rgame/render_surface.h"
#include "rgame/render_shader.h"

void create_render_shader_copy(render_shader** result, render_shader* source, lifetime_memory_manager* memory)
{
	render_shader* new_shader = lifetime_memory_manager::allocate<render_shader>(memory, "");

	render_shader::create(new_shader);

	for (auto i : source->sources)
	{
		render_shader::append_shader_source(new_shader, i.shader_type, i.source);
	}

	*result = new_shader;
}

void create_render_surface_copy(render_surface** result, render_surface* source, lifetime_memory_manager* memory, bool copy_shader)
{
	render_surface* new_surface = lifetime_memory_manager::allocate<render_surface>(memory, "");

	render_surface::copy(new_surface, source);

	if (copy_shader)
	{
		create_render_shader_copy(&new_surface->working_shader, source->working_shader, memory);
	}

	*result = new_surface;
}
