#pragma once

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <vector>
#include <map>

struct render_window;

struct press_data
{
	bool last_down;
	bool is_down;
};

struct input_manager
{
	render_window*				window;
	std::map<int, press_data>	requested_keys;

	int							mouse_x;
	int							mouse_y;
	int							mouse_velocity_x;
	int							mouse_velocity_y;

	static void create(input_manager* result, render_window* window);
	static void destroy(input_manager* constext);
	static void update(input_manager* input_manager_context);

	static bool get_key_pressed(input_manager* input_manager_context, int key);
	static bool get_key_down(input_manager* input_manager_context, int key);
	static bool get_key_up(input_manager* input_manager_context, int key);

	static void lock_mouse(input_manager* input_manager_context);
	static void unlock_mouse(input_manager* input_manager_context);
};

#endif