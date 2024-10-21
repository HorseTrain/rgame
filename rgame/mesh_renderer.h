#pragma once

#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

struct render_surface;

struct mesh_renderer
{
	render_surface* render_surface_context;
	void*			mesh_context;

	void*			upload_function;
	void*			bind_function;
	void*			generic_draw_function;

	template		<typename T>
	static void		create(mesh_renderer* result, render_surface* render_surface_context, T* mesh_context)
	{
		result->render_surface_context = render_surface_context;

		result->mesh_context = mesh_context;

		result->upload_function = T::upload;
		result->bind_function = T::bind;
		result->generic_draw_function = T::generic_draw;
	}

	static void		draw(mesh_renderer* mesh_renderer_context);
};

#endif