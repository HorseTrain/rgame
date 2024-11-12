#pragma once

#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <string>

#include "input_manager.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

struct render_window;

typedef void(*window_function)(render_window* window, void* arguments);

struct render_window
{
	window_function start;
	window_function loop;
	window_function end;

	uint32_t		window_width;
	uint32_t		window_height;

	float			delta_time;
	float			target_framerate;

	bool			force_close;

	input_manager	input_manager_context;

	GLFWwindow*		raw_window;

	static void		create(render_window* result, window_function start, window_function loop, window_function end, uint32_t window_width, uint32_t window_height, float target_framerate, std::string window_name);
	static void		create(render_window* result, window_function loop, uint32_t window_width, uint32_t window_height, float target_framerate, std::string window_name);
	static void		destroy(render_window* window, bool destroy_glfw_global = true);
	static void		make_context_current(render_window* window);
	static bool		should_window_close(render_window* window);
	static void		poll_events(render_window* window);
	static void		swap_buffers(render_window* window);
	static float	get_aspect_ratio(render_window* window);
	static void		set_resizable(render_window* window, bool is_resizable);

	static void		run(render_window* window, void* arguments);
};

#endif