#include "debug_renderer.h"

#include "mario.h"

#include "game_objects/camera.h"

void debug_renderer::create(debug_renderer** debug_renderer_context, level* level_context, camera* camera_context)
{
	game_object* working_object;
	game_object::create(&working_object, "", level_context);

	debug_renderer* result = game_object::allocate_game_object_data<debug_renderer>(working_object);

	result->camera_context = camera_context;

	transform::create_identity(&result->object_transform);

	*debug_renderer_context = result;
}

void debug_renderer::start(debug_renderer* debug_renderer_context)
{
	render_surface* my_surface;

	create_render_surface_copy(&my_surface, lifetime_memory_manager::get_allocation<render_surface>(&debug_renderer_context->game_object_context->game_context->memory, "debug_surface"), &debug_renderer_context->game_object_context->memory, true);

	mesh_renderer::create<static_render_mesh>(
		&debug_renderer_context->mesh_renderer_context,
		my_surface,
		lifetime_memory_manager::get_allocation<static_render_mesh>(&debug_renderer_context->game_object_context->game_context->memory, "debug_triangle")
	);
}

void debug_renderer::update(debug_renderer* debug_renderer_context)
{
	render_surface::set_data<glm::mat4>(debug_renderer_context->mesh_renderer_context.render_surface_context, "camera_transform", camera::get_camera_perspective(debug_renderer_context->camera_context), render_surface_data_type::mat4x4);
	render_surface::set_data<glm::mat4>(debug_renderer_context->mesh_renderer_context.render_surface_context, "object_transform", transform::get_world_transform_matrix_recursive(&debug_renderer_context->object_transform), render_surface_data_type::mat4x4);

	mesh_renderer::draw(&debug_renderer_context->mesh_renderer_context);
}

void debug_renderer::destroy(debug_renderer* debug_renderer_context)
{

}
