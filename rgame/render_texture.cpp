#include "render_texture.h"
#include "render_window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void render_texture::create(render_texture* result)
{
	result->texture_buffer = nullptr;
	result->gl_handle = -1;
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

	glGenTextures(1, (GLuint*) & render_texture_context->gl_handle);
	glBindTexture(GL_TEXTURE_2D, render_texture_context->gl_handle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, render_texture_context->width, render_texture_context->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, render_texture_context->texture_buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
}

void render_texture::use(render_texture* render_texture_context, int texture_index)
{
	glActiveTexture(render_texture_context->gl_handle);
	glBindTexture(GL_TEXTURE_2D, render_texture_context->gl_handle);
}
