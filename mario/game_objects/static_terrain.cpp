#include "static_terrain.h"
#include "mario.h"

void static_terrain::create(static_terrain** result, level* level_context)
{
	game_object* game_object_context;
	game_object::create(&game_object_context, "", level_context);

	static_terrain* static_terrain_mesh_context = game_object::allocate_game_object_data< static_terrain>(game_object_context);

	transform::create_identity(&static_terrain_mesh_context->object_transform);

	*result = static_terrain_mesh_context;
}

void static_terrain::start(static_terrain* static_terrain_mesh_context)
{
	game_object* game_object_context = static_terrain_mesh_context->game_object_context;

	static_render_mesh* debug_plane;
	render_surface* my_surface;

	debug_plane = lifetime_memory_manager::allocate<static_render_mesh>(&game_object_context->memory);
	static_render_mesh::create_debug_plane(debug_plane, 100);

	create_render_surface_copy(&my_surface, lifetime_memory_manager::get_allocation<render_surface>(&game_object_context->game_context->memory, "debug_surface"), &game_object_context->memory, true);

	mesh_renderer::create<static_render_mesh>(&static_terrain_mesh_context->terrain_renderer, my_surface, debug_plane);
}

void static_terrain::update(static_terrain* static_terrain_mesh_context)
{

}

void static_terrain::destroy(static_terrain* static_terrain_mesh_context)
{

}
