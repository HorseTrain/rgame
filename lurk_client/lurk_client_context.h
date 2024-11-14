#pragma once

#include <string>
#include <vector>

#include "rgame/glm/vec2.hpp"

#ifndef LURK_CLIENT_CONTEXT_H
#define LURK_CLIENT_CONTEXT_H

#define TEXT_BOARD_COUNT		6

#define MAIN_BOARD				0
#define MESSAGE_BOARD			1
#define	CONNECTED_ROOM_BOARD	2
#define PLAYER_MONSTER_BOARD	3
#define STATUS_BOARD			4
#define COMMAND_BOARD			5

struct lurk_client;
struct render_window;
struct io;
struct software_text_renderer;
struct font_atlas;
struct render_shader;
struct debug_mesh;

struct game_data
{
	int			initial_points;
	int			stat_min;
	int			stat_max;

	std::string description;
};

struct message_board
{
	software_text_renderer* software_text_renderer_context;
	glm::ivec2				offset;

	static void				create(message_board* result, glm::ivec2 offset, int width, int height, font_atlas* font_atlas_context);
	static void				destroy(message_board* to_destroy);
};

struct lurk_client_context
{
	lurk_client*			raw_client;
	std::vector<int>		version;

	render_window*			render_window_context;
	io*						io_context;

	debug_mesh*				quad;

	font_atlas*				cascadia;

	render_shader*			text_renderer_shader;

	message_board*			boards[TEXT_BOARD_COUNT];

	static void				create(lurk_client_context* result, int port, std::string name, render_window* render_window_context, std::string executable_path);
	static void				destroy(lurk_client_context* lurk_client_context_context);

	static void				run(lurk_client_context* lcc);

	static void				draw_boards(lurk_client_context* lcc);
};

#endif