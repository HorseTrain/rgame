#include "software_text_renderer.h"
#include "render_texture.h"
#include "pixel.h"
#include "font_atlas.h"
#include "rgame/glm/vec2.hpp"
#include "rgame/glm/vec4.hpp"

static void set_pixel(render_texture* result, int x, int y, rgba_i8 data)
{
	*rgba_i8::get_pixel((rgba_i8*)result->texture_buffer, x, y, result->width) = data;
}

static rgba_i8 get_pixel(render_texture* result, int x, int y)
{
	return *rgba_i8::get_pixel((rgba_i8*)result->texture_buffer, x, y, result->width);
}

static bool in_bound(glm::ivec2 pixel, glm::ivec2 size)
{
	for (int i = 0; i < 2; ++i)
	{
		if (pixel[i] < 0)
			return false;

		if (pixel[i] >= size[i])
			return false;
	}

	return true;
}

static rgba_i8 get_pixel_blur(render_texture* source, int x, int y, int blur)
{
	glm::vec4 result_color = glm::vec4(0);

	int avg = 0;

	for (int oy = 0; oy < blur; ++oy)
	{
		for (int ox = 0; ox < blur; ++ox)
		{
			glm::ivec2 working_source_position = glm::ivec2(x, y) + (glm::ivec2(ox, oy) - glm::ivec2(blur / 2));

			if (!in_bound(working_source_position, glm::vec2(source->width, source->height)))
				continue;

			rgba_i8 working_color = get_pixel(source, working_source_position.x, working_source_position.y);

			result_color += glm::vec4(working_color.data[0], working_color.data[1], working_color.data[2], working_color.data[3]);

			avg++;
		}
	}

	result_color /= avg;

	return { (uint8_t)result_color.x, (uint8_t)result_color.y , (uint8_t)result_color.z , (uint8_t)result_color.w };
}

static void write_texture(render_texture* result, render_texture* source, glm::ivec2 result_offset, glm::ivec2 source_offset, glm::ivec2 source_size, float scale, int* x_advance)
{
	source_size = (glm::ivec2)((glm::vec2)source_size * scale);

	int blur = 3;

	for (int y = 0; y < source_size.y; ++y)
	{
		for (int x = 0; x < source_size.x; ++x)
		{
			glm::ivec2 result_location = result_offset + glm::ivec2(x, y);
			glm::ivec2 source_location = glm::ivec2(source_offset.x, source_offset.y) + (glm::ivec2)(glm::vec2(x, y) / scale);

			if (!in_bound(result_location, glm::vec2(result->width, result->height)) ||
				!in_bound(source_location, glm::vec2(source->width, source->height))
				)
				return;

			rgba_i8 source_pixel = get_pixel_blur(source, source_location.x, source_location.y, blur);

			if (source_pixel.data[3] == 0)
				continue;

			set_pixel(result, result_location.x, result_location.y, source_pixel);
		}
	}

	*x_advance = (int)(*x_advance * scale);
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

void software_text_renderer::render_text(software_text_renderer* str, std::string test, text_mode mode)
{
	font_atlas* working_atlas = str->font_atlas_context;

	float scale = 0.2f;

	switch (mode)
	{
	case left_right:
	{
		for (int i = 0; i < test.size(); ++i)
		{
			char working_char = test[i];

			if (working_char == ' ')
			{
				str->x += working_atlas->average_x_advance * scale;
			}
			else if (working_char == '\n')
			{
				str->y += working_atlas->max_y_advance * scale;
				str->x = 0;
			}

			if (working_atlas->entries.find(working_char) == working_atlas->entries.end())
			{
				continue;
			}

			font_entry* entry = &working_atlas->entries[test[i]];

			int x_advance = entry->x_advance;

			glm::vec2 letter_offset = glm::ivec2(str->x, str->y) + (glm::ivec2)(glm::vec2(entry->x_offset, entry->y_offset) * scale);

			write_texture(
				str->backing_texture,
				working_atlas->font_atlas_texture,
				letter_offset,
				glm::ivec2(entry->x, entry->y),
				glm::ivec2(entry->width, entry->height),
				scale,
				&x_advance
			);

			str->x += x_advance;
		}
	}; break;

	default: throw 0;
	}
}
