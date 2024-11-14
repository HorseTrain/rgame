#pragma once

#include <string>

#ifndef SOFTWARE_TEXT_RENDERER_H
#define SOFTWARE_TEXT_RENDERER_H

struct render_texture;
struct font_atlas;

enum text_mode
{
	right_left,
	center,
	left_right
};

struct software_text_renderer
{
	render_texture* backing_texture;
	font_atlas*		font_atlas_context;

	int				width;
	int				height;

	float			scale;

	int x;
	int y;

	static void		create(software_text_renderer* result, int texture_width, int texture_height, float scale, font_atlas* font_atlas_context);
	static void		destroy(software_text_renderer* to_destroy);

	static void		reset(software_text_renderer* software_text_renderer_context);
	static void		render_text(software_text_renderer* software_text_renderer_context, std::string test, text_mode mode);
};

#endif