#pragma once

#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

struct render_window;

typedef void(*window_main_loop)(render_window* window, uint64_t frame_draw_time, void* arguments);

struct render_window
{
	window_main_loop loop;

	uint32_t		window_width;
	uint32_t		window_height;

	float			delta_time;
	float			target_framerate;

	GLFWwindow*		raw_window;

	static void		create(render_window* result, window_main_loop loop, uint32_t window_width, uint32_t window_height, float target_framerate, std::string window_name);
	static void		destroy(render_window* window);
	static void		make_context_current(render_window* window);
	static bool		should_window_close(render_window* window);
	static void		poll_events(render_window* window);
	static void		swap_buffers(render_window* window);
	static float	get_aspect_ratio(render_window* window);

	static void		run(render_window* window, void* arguments);
};

#endif