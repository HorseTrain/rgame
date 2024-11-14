#include "render_texture.h"
#include "render_window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static void upload_texture(render_texture* render_texture_context)
{
	if (render_texture_context->texture_buffer == nullptr)
		return;

	if (render_texture_context->gl_handle != -1)
		return;

	glGenTextures(1, (GLuint*)&render_texture_context->gl_handle);
	glBindTexture(GL_TEXTURE_2D, render_texture_context->gl_handle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, render_texture_context->width, render_texture_context->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, render_texture_context->texture_buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
}

void render_texture::create(render_texture* result)
{
	result->texture_buffer = nullptr;
	result->gl_handle = -1;
}

void render_texture::create_empty(render_texture* result, int width, int height, int pixel_size)
{
	create(result);

	result->width = width;
	result->height = height;

	int texture_buffer_size = width * height * pixel_size;

	result->texture_buffer = malloc(texture_buffer_size);

	for (int i = 0; i < texture_buffer_size; ++i)
	{
		*(uint8_t*)((char*)result->texture_buffer + i) = 255;
	}
}

void render_texture::clear_color(render_texture* result, int pixel_size)
{
	int texture_buffer_size = result->width * result->height * pixel_size;

	for (int i = 0; i < texture_buffer_size; ++i)
	{
		*(uint8_t*)((char*)result->texture_buffer + i) = 0;
	}
}

void render_texture::unload(render_texture* render_texture_context)
{
	if (render_texture_context->gl_handle == -1)
		return;

	glDeleteTextures(1, (GLuint*)&render_texture_context->gl_handle);

	render_texture_context->gl_handle = -1;
}

void render_texture::destroy(render_texture* render_texture_context)
{
	if (render_texture_context->texture_buffer != nullptr)
	{
		free(render_texture_context->texture_buffer);
	}

	if (render_texture_context->gl_handle != -1)
	{
		glDeleteTextures(1, (GLuint*) & render_texture_context->gl_handle);
	}
}

void render_texture::load_from_file(render_texture* render_texture_context, std::string path)
{
	int cbi;
	render_texture_context->texture_buffer = (char*)stbi_load(path.c_str(), &render_texture_context->width, &render_texture_context->height, &cbi, 4);
}

void render_texture::use(render_texture* render_texture_context, int texture_index)
{
	upload_texture(render_texture_context);

	glActiveTexture(render_texture_context->gl_handle);
	glBindTexture(GL_TEXTURE_2D, render_texture_context->gl_handle);
}
