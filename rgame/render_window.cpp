#include "render_window.h"

#include <chrono>
#include <iostream>

static bool glfw_context_open = false;

void render_window::create(render_window* result, window_main_loop loop, uint32_t window_width, uint32_t window_height,float target_framerate, std::string window_name)
{
	result->loop = loop;
	result->target_framerate = target_framerate;

	if (!glfw_context_open)
	{
		glfwInit();

		glfw_context_open = true;
	}
	
	result->raw_window = glfwCreateWindow(window_width, window_height, window_name.c_str(), NULL, NULL);

	glfwMakeContextCurrent(result->raw_window);

	glewInit();
}

void render_window::destroy(render_window* window)
{
	glfwDestroyWindow(window->raw_window);
}

void render_window::make_context_current(render_window* window)
{
	glfwMakeContextCurrent(window->raw_window);
}

bool render_window::should_window_close(render_window* window)
{
	return glfwWindowShouldClose(window->raw_window);
}

void render_window::run(render_window* window, void* arguments)
{
	make_context_current(window);

	window->delta_time = 1;

	while (!should_window_close(window))
	{
		auto start_time = std::chrono::high_resolution_clock::now();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(.5, .5, .5, 1);

		poll_events(window);

		glfwGetWindowSize(window->raw_window, (int*) & window->window_width, (int*) & window->window_height);
		glViewport(0, 0, window->window_width, window->window_height);

		if (window->loop != nullptr)
		{
			window->loop(window, 0, arguments);
		}

		swap_buffers(window);

		auto end_time = std::chrono::high_resolution_clock::now();

		auto elapsed_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

		double framerate = (1000000000.0 / (double)elapsed_nanoseconds);
		double delta_time = (double)window->target_framerate / framerate;

		window->delta_time = (float)delta_time;
	}
}

void render_window::poll_events(render_window* window)
{
	make_context_current(window);

	glfwPollEvents();
}

void render_window::swap_buffers(render_window* window)
{
	//make_context_current(window);

	glfwSwapBuffers(window->raw_window);
}

float render_window::get_aspect_ratio(render_window* window)
{
	return (float)window->window_width / (float)window->window_height;
}
