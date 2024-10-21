#include "mesh_renderer.h"
#include "render_surface.h"

void mesh_renderer::draw(mesh_renderer* mesh_renderer_context)
{
	((void(*)(void*))mesh_renderer_context->upload_function)(mesh_renderer_context->mesh_context);
	((void(*)(void*))mesh_renderer_context->bind_function)(mesh_renderer_context->mesh_context);

	render_surface::use(mesh_renderer_context->render_surface_context);

	((void(*)(void*))mesh_renderer_context->generic_draw_function)(mesh_renderer_context->mesh_context);
}
