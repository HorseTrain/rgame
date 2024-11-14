#include "lurk_client_context.h"
#include "lurk/lurk_client.h"
#include "rgame/font_atlas.h"
#include "rgame/software_text_renderer.h"
#include "rgame/io.h"
#include "rgame/render_shader.h"
#include "rgame/shader_compiler.h"
#include "rgame/render_window.h"
#include "rgame/debug_mesh.h"
#include "rgame/render_texture.h"
#include <assert.h>

#include "lurk/lurk_structs.h"

#define FONT_ATLAS_PATH			"C:\\Users\\raymo\OneDrive\\Desktop\\font\\cascadia.fnt"
#define FONT_ATLAS_TEXTURE_PATH "C:\\Users\\raymo\OneDrive\\Desktop\\font\\cascadia.png"

struct message_board_data
{
	glm::vec2 offset;
	glm::vec2 size;
};

static void read_lurk_version(lurk_client_context* lcc)
{
	lurk_client* raw_client = lcc->raw_client;

	lurk_version version_header;

	lurk_client::read_data_raw(&version_header, sizeof(lurk_version), raw_client);

	lcc->version = std::vector<int>({ version_header.major, version_header.minor });

	int minor_count = version_header.extension_list;

	assert(minor_count == 0);

	if (minor_count != 0)
	{
		throw 0;
	}
}

static void read_lurk_description(lurk_client_context* lcc)
{
	lurk_client* raw_client = lcc->raw_client;

	lurk_game game_header;

	lurk_client::read_data_raw(&game_header, sizeof(lurk_game), raw_client);

	char* message_buffer = (char*)malloc(game_header.description_length);

	lurk_client::read_data_raw(message_buffer, game_header.description_length, raw_client);

	free(message_buffer);
}

static void create_io_context(lurk_client_context* result, std::string executable_path)
{
	result->io_context = new io;

	io::get_parent_directory(&executable_path, executable_path);
	io::create(result->io_context, executable_path);
}

static void create_text_renderers(lurk_client_context* result)
{
	std::string font_texture_path = io::get_path(result->io_context, "cascadia.png");
	std::string font_atlas_path = io::get_path(result->io_context, "cascadia.fnt");

	render_window* window = result->render_window_context;

	result->cascadia = new font_atlas;
	result->text_renderer_shader = new render_shader;

	font_atlas::create_from_file(result->cascadia, result->io_context, font_atlas_path, font_texture_path);

	get_shader_from_path(result->text_renderer_shader, "text_shader", result->io_context);
	render_shader::compile(result->text_renderer_shader);

	debug_mesh::create_debug_quad(&result->quad);

	int boarder = 3;

	int width = result->render_window_context->window_width;
	int height = result->render_window_context->window_height;

	int top_item_height = height - 200;

	message_board_data boards[] = {

		//Main
		{
			glm::ivec2(boarder, boarder),
			glm::ivec2(width - 300 - (boarder / 2), top_item_height)
		},

		//Message
		{
			glm::ivec2(width - 300 + (boarder / 2), boarder),
			glm::ivec2(width - boarder, boarder + 300 - (boarder / 2))
		},

		//Connected Rooms
		{
			glm::ivec2(width - 300 + (boarder / 2), boarder + 300 + (boarder / 2)),
			glm::ivec2(width - boarder, top_item_height)
		},

		//Player Monster
		{
			glm::ivec2(boarder + 400 + boarder + 300 + boarder, top_item_height + boarder),
			glm::ivec2(width - boarder, height - boarder)
		},

		//Status
		{
			glm::ivec2(boarder + 400 + boarder, top_item_height + boarder),
			glm::ivec2(boarder + 400 + boarder + 300, height - boarder)
		},

		//Command
		{
			glm::ivec2(boarder, top_item_height + boarder),
			glm::ivec2(boarder + 400, height - boarder)
		}
	};

	for (int i = 0; i < TEXT_BOARD_COUNT; ++i)
	{
		result->boards[i] = new message_board;

		message_board::create(result->boards[i], boards[i].offset, boards[i].size.x - boards[i].offset.x, boards[i].size.y - boards[i].offset.y, result->cascadia);
	}
}

void lurk_client_context::create(lurk_client_context* result, int port, std::string name, render_window* render_window_context, std::string executable_path)
{
	result->raw_client = new lurk_client();

	result->render_window_context = render_window_context;

	create_io_context(result, executable_path);
	create_text_renderers(result);

	lurk_client::create(result->raw_client, port, name);
}

void lurk_client_context::destroy(lurk_client_context* lurk_client_context_context)
{
	font_atlas::destroy(lurk_client_context_context->cascadia);
	render_shader::destroy(lurk_client_context_context->text_renderer_shader);

	delete lurk_client_context_context->raw_client;
	delete lurk_client_context_context->cascadia;
	delete lurk_client_context_context->text_renderer_shader;

	delete lurk_client_context_context->io_context;

	for (int i = 0; i < TEXT_BOARD_COUNT; ++i)
	{
		message_board::destroy(lurk_client_context_context->boards[i]);

		delete lurk_client_context_context->boards[i];
	}

	debug_mesh::destroy(lurk_client_context_context->quad); 
	delete lurk_client_context_context->quad;

	lurk_client::send_leave(lurk_client_context_context->raw_client);
	lurk_client::destroy(lurk_client_context_context->raw_client);
}

void lurk_client_context::run(lurk_client_context* lcc)
{
	read_lurk_version(lcc);

	read_lurk_description(lcc);
}

static void draw_board(lurk_client_context* lcc, message_board* board)
{
	render_shader* shader_in_use = lcc->text_renderer_shader;
	render_window* working_window = lcc->render_window_context;
	debug_mesh* mesh_in_use = lcc->quad;

	float window_aspect = render_window::get_aspect_ratio(working_window);

	render_shader::use(shader_in_use);

	render_texture::unload(board->software_text_renderer_context->backing_texture);

	render_texture::use(board->software_text_renderer_context->backing_texture, 0);

	glm::vec2 offset = ((glm::vec2)board->offset - glm::vec2(working_window->window_width / 2.0, working_window->window_height / 2.0)) / (glm::vec2(working_window->window_width, working_window->window_height) / 2.0f);
	glm::vec2 scale = glm::vec2((1.0 / working_window->window_width) * board->software_text_renderer_context->width, (1.0 / working_window->window_height) * board->software_text_renderer_context->height) * 2.0f;

	render_shader::uniform_vec2(shader_in_use, "offset", offset);
	render_shader::uniform_vec2(shader_in_use, "scale", scale);

	debug_mesh::generic_draw(mesh_in_use);
}

void lurk_client_context::draw_boards(lurk_client_context* lcc)
{
	for (int i = 0; i < TEXT_BOARD_COUNT; ++i)
	{
		software_text_renderer::reset(lcc->boards[i]->software_text_renderer_context);
	}

	software_text_renderer::render_text(lcc->boards[0]->software_text_renderer_context, "Amongus\nSussy\nImpasta", text_mode::left_right);
	software_text_renderer::render_text(lcc->boards[1]->software_text_renderer_context, "SUS", text_mode::left_right);

	for (int i = 0; i < TEXT_BOARD_COUNT; ++i)
	{
		draw_board(lcc, lcc->boards[i]);
	}
}

void message_board::create(message_board* result, glm::ivec2 offset, int width, int height, font_atlas* font_atlas_context)
{
	result->software_text_renderer_context = new software_text_renderer;
	software_text_renderer::create(result->software_text_renderer_context, width, height, 1, font_atlas_context);

	software_text_renderer::reset(result->software_text_renderer_context);

	result->offset = offset;
}

void message_board::destroy(message_board* to_destroy)
{
	software_text_renderer::destroy(to_destroy->software_text_renderer_context);
	delete to_destroy->software_text_renderer_context;
}