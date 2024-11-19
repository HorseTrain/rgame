#include "software_text_renderer.h"
#include "render_texture.h"
#include "pixel.h"
#include "font_atlas.h"
#include "rgame/glm/vec2.hpp"
#include "rgame/glm/vec4.hpp"

struct letter
{
	font_entry* entry;
	char		id;

	glm::vec2	result_offset;
	float		scale;
};

static bool in_bound(glm::ivec2 pixel, glm::ivec2 size)
{
	return !(pixel.x < 0 || pixel.x >= size.x || pixel.y < 0 || pixel.y >= size.y);
}

static bool in_bound(render_texture* render_texture_context, glm::ivec2 point)
{
	return in_bound(point, glm::ivec2(render_texture_context->width, render_texture_context->height));
}

static void set_pixel(render_texture* result, int x, int y, rgba_i8 data)
{
	if (!in_bound(result, glm::ivec2(x, y)))
		return;

	*rgba_i8::get_pixel((rgba_i8*)result->texture_buffer, x, y, result->width) = data;
}

static rgba_i8 get_pixel(render_texture* result, int x, int y, font_entry* working_entry)
{
	if (working_entry != nullptr)
	{
		if
			(x < working_entry->x || x >= (working_entry->x + working_entry->width) ||
			(y < working_entry->y || y >= (working_entry->y + working_entry->height)
		))
		return { 0,0,0,0 };
	}

	return *rgba_i8::get_pixel((rgba_i8*)result->texture_buffer, x, y, result->width);
}

static rgba_i8 get_pixel_blur(render_texture* source, int x, int y, int blur, font_entry* working_atlas = nullptr)
{
	if (blur == 0)
	{
		return get_pixel(source, x, y, working_atlas);
	}

	glm::vec4 result_color = glm::vec4(0);

	int avg = 0;

	for (int oy = -blur; oy <= blur; ++oy)
	{
		for (int ox = -blur; ox <= blur; ++ox)
		{
			glm::ivec2 working_source_position = glm::ivec2(x, y) + glm::ivec2(ox, oy);

			if (!in_bound(working_source_position, glm::vec2(source->width, source->height)))
			{
				continue;
			}

			rgba_i8 working_color = get_pixel(source, working_source_position.x, working_source_position.y, working_atlas);

			result_color += glm::vec4(working_color.data[0], working_color.data[1], working_color.data[2], working_color.data[3]);

			avg++;
		}
	}

	result_color /= avg;

	return { (uint8_t)result_color.x, (uint8_t)result_color.y , (uint8_t)result_color.z , (uint8_t)result_color.w };
}

static void write_character_to_buffer(render_texture* result, letter* to_write, int blur, float scale)
{
	if (to_write->entry == nullptr)
		return;

	font_entry* entry = to_write->entry;

	glm::vec2 x_advance = glm::vec2(-blur, entry->width + blur);
	glm::vec2 y_advance = glm::vec2(-blur, entry->height + blur);

	float append = 1.0 / scale;

	for (float y = y_advance.x; y < y_advance.y; y += append)
	{
		for (float x = x_advance.x; x < x_advance.y; x += append)
		{
			int ix = (int)x;
			int iy = (int)y;

			glm::ivec2 result_offset = to_write->result_offset + glm::vec2(ix + entry->x_offset, iy + entry->y_offset) * scale;

			if (!in_bound(result, { ix, iy }))
			{
				continue;
			}

			rgba_i8 source_color = get_pixel_blur(entry->font_atlas_context->font_atlas_texture, entry->x + ix,entry->y + iy, blur, entry);

			set_pixel(result, result_offset.x, result_offset.y, source_color);
		}
	}
}

void software_text_renderer::create(software_text_renderer* result, int texture_width, int texture_height, float scale, font_atlas* font_atlas_context)
{
	result->width = texture_width;
	result->height = texture_height;

	result->scale = scale;

	result->backing_texture = new render_texture();

	result->font_atlas_context = font_atlas_context;

	render_texture::create_empty(result->backing_texture, texture_width, texture_height, sizeof(rgba_i8));
}

void software_text_renderer::destroy(software_text_renderer* to_destroy)
{
	render_texture::destroy(to_destroy->backing_texture);

	delete to_destroy->backing_texture;
}

void software_text_renderer::reset(software_text_renderer* software_text_renderer_context)
{
	software_text_renderer_context->x = 0;
	software_text_renderer_context->y = 0;

	render_texture::clear_color(software_text_renderer_context->backing_texture, sizeof(rgba_i8));
}

void software_text_renderer::render_text(software_text_renderer* str, std::string test, text_mode mode, int pixel_size)
{
	font_atlas* working_atlas = str->font_atlas_context;

	std::vector<letter> letters;
	std::vector<float> line_lengths;

	float scale = (float)pixel_size / str->font_atlas_context->max_y_advance ;

	float line_length = 0;

	for (int i = 0; i < test.size(); ++i)
	{
		char working_character = test[i];

		letter working_letter;

		font_entry* entry;

		float advance = 0;

		if (str->font_atlas_context->entries.find(working_character) != str->font_atlas_context->entries.end())
		{
			entry = &str->font_atlas_context->entries[working_character];

			advance = entry->x_advance * scale;
		}
		else
		{
			entry = nullptr;
		}

		working_letter.entry = entry;
		working_letter.scale = scale;

		if ((str->x + advance) >= str->width && working_character != '\n')
		{
			line_lengths.push_back(str->x);

			str->x = 0;
			str->y += str->font_atlas_context->max_y_advance * scale;
		}

		working_letter.id = working_character;
		working_letter.result_offset = glm::vec2(str->x, str->y);
		working_letter.scale = scale;

		if (working_character == ' ')
		{
			str->x += str->font_atlas_context->average_x_advance * scale;
		}
		else if (working_character == '\n')
		{
			line_lengths.push_back(str->x);

			str->x = 0;
			str->y += str->font_atlas_context->max_y_advance * scale;
		}
		else
		{
			str->x += advance;
		}

		letters.push_back(working_letter);
	}

	line_lengths.push_back(str->x);

	switch (mode)
	{
	case center:
	{
		int line = 0;

		for (int i = 0; i < letters.size(); ++i)
		{
			letter* working_letter = &letters[i];

			working_letter->result_offset.x += (str->width / 2) - line_lengths[line] / 2;

			if (working_letter->id == '\n')
			{
				line++;
			}
		}
	}; break;

	case right_left:
	{
		int line = 0;

		for (int i = 0; i < letters.size(); ++i)
		{
			letter* working_letter = &letters[i];

			working_letter->result_offset.x += str->width - line_lengths[line];

			if (working_letter->id == '\n')
			{
				line++;
			}
		}
	}; break;
	}

	int blur = 1;

	for (int i = 0; i < letters.size(); ++i)
	{
		write_character_to_buffer(str->backing_texture, &letters[i], blur, scale);
	}
}
