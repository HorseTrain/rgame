#include "font_atlas.h"
#include "string_tools.h"
#include "render_texture.h"
#include "io.h"

#include <assert.h>

static int get_equal(std::string source)
{
	std::vector<std::string> parts;

	string_split(&parts, source, '=');

	return std::stoi(parts[1]);
}

static void create_entry(font_atlas* result, std::string line)
{
	std::vector<std::string> line_parts;

	string_split(&line_parts, line, ' ');

	if (line_parts[0] != "char")
		return;

	font_entry entry;

	entry.id = get_equal(line_parts[1]);

	entry.x = get_equal(line_parts[2]);
	entry.y = get_equal(line_parts[3]);

	entry.width = get_equal(line_parts[4]);
	entry.height = get_equal(line_parts[5]);

	entry.x_offset = get_equal(line_parts[6]);
	entry.y_offset = get_equal(line_parts[7]);

	entry.x_advance = get_equal(line_parts[8]);

	assert(result->entries.find(entry.id) == result->entries.end());

	entry.font_atlas_context = result;

	result->average_x_advance += entry.x_advance;

	if (entry.height >= result->max_y_advance)
	{
		result->max_y_advance = entry.height;
	}

	result->entries[entry.id] = entry;
}

static void create(font_atlas* result, std::string font_atlas,render_texture* font_atlas_texture)
{
	while (1)
	{
		int og_length = font_atlas.size();

		string_replace(&font_atlas, font_atlas, "  ", " ");

		if (font_atlas.size() == og_length)
			break;
	}

	std::vector<std::string> lines;

	string_split(&lines, font_atlas, '\n');

	result->average_x_advance = 0;
	result->max_y_advance = 0;

	for (int i = 0; i < lines.size(); ++i)
	{
		create_entry(result, lines[i]);
	}

	result->font_atlas_texture = font_atlas_texture;
	result->average_x_advance /= result->entries.size();
}

void font_atlas::create_from_file(font_atlas* result, io* io_context, std::string atlas_path, std::string texture_path)
{
	char* atlas_buffer;
	uint64_t atlas_buffer_size;

	io::io_read_entire_file(io_context, &atlas_buffer, &atlas_buffer_size, atlas_path);

	std::string atlas_string_buffer = std::string(atlas_buffer, atlas_buffer_size);
	free(atlas_buffer);

	render_texture* font_atlas_texture = new render_texture();

	io::fix_path(&texture_path, texture_path);
	render_texture::load_from_file(font_atlas_texture, texture_path);

	create(result, atlas_string_buffer, font_atlas_texture);
}

void font_atlas::destroy(font_atlas* to_destroy)
{
	render_texture::destroy(to_destroy->font_atlas_texture);

	delete to_destroy->font_atlas_texture;
}