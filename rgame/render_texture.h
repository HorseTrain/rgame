#pragma once

#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include <string>

struct render_texture
{
	void*	texture_buffer;
	int		gl_handle;

	int		width;
	int		height;

	static void		create(render_texture* result);
	static void		create_empty(render_texture* result, int width, int height, int pixel_size);
	static void		clear_color(render_texture* result, int pixel_size);
	static void		unload(render_texture* render_texture_context);
	static void		destroy(render_texture* render_texture_context);
	static void		load_from_file(render_texture* render_texture_context,std::string path);
	static void		use(render_texture* render_texture_context, int texture_index);
};

#endif