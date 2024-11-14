#pragma once

#include <string>
#include <unordered_map>

#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H

struct io;
struct render_texture;

struct font_entry
{
	int id;

	int x;
	int y;
	
	int width;
	int height;

	int x_offset;
	int y_offset;

	int x_advance;
};

struct font_atlas
{
	std::unordered_map<int,font_entry>	entries;
	render_texture*						font_atlas_texture;

	int									average_x_advance;
	int									max_y_advance;

	static void create_from_file(font_atlas* result,io* io_context, std::string path, std::string texture_path);
	static void destroy(font_atlas* to_destroy);
};

#endif