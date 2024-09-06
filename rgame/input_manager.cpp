#include "input_manager.h"
#include "render_window.h"

void input_manager::create(input_manager* result, render_window* window)
{
	result->window = window;
	result->requested_keys = std::map<int, press_data>();
}

void input_manager::update(input_manager* input_manager_context)
{
	std::map<int, press_data>* key_data_ptr = &input_manager_context->requested_keys;

	for (auto i = key_data_ptr->begin(); i != key_data_ptr->end(); ++i)
	{
		int key				= i->first;
		press_data* data_ptr	= &i->second;

		data_ptr->last_down = data_ptr->is_down;
		data_ptr->is_down = glfwGetKey(input_manager_context->window->raw_window, key);
	}

	double mouse[2];
	glfwGetCursorPos(input_manager_context->window->raw_window, &mouse[0], &mouse[1]);

	input_manager_context->mouse_x = mouse[0];
	input_manager_context->mouse_y = mouse[1];
}

void input_manager::register_key(input_manager* input_manager_context, int key)
{
	std::map<int, press_data>* key_data_ptr = &input_manager_context->requested_keys;

	(*key_data_ptr)[key] = { 0, 0 };
}

bool input_manager::get_key_pressed(input_manager* input_manager_context, int key)
{
	std::map<int, press_data>* key_data_ptr = &input_manager_context->requested_keys;

	if (key_data_ptr->find(key) == key_data_ptr->end())
		return false;

	press_data* data_ptr = &(*key_data_ptr)[key];

	return !data_ptr->last_down && data_ptr->is_down;
}

bool input_manager::get_key_down(input_manager* input_manager_context, int key)
{
	std::map<int, press_data>* key_data_ptr = &input_manager_context->requested_keys;

	if (key_data_ptr->find(key) == key_data_ptr->end())
		return false;

	press_data* data_ptr = &(*key_data_ptr)[key];

	return data_ptr->is_down;
}

bool input_manager::get_key_up(input_manager* input_manager_context, int key)
{
	std::map<int, press_data>* key_data_ptr = &input_manager_context->requested_keys;

	if (key_data_ptr->find(key) == key_data_ptr->end())
		return false;

	press_data* data_ptr = &(*key_data_ptr)[key];

	return data_ptr->last_down && !data_ptr->is_down;
}

void input_manager::lock_mouse(input_manager* input_manager_context)
{
	glfwSetInputMode(input_manager_context->window->raw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void input_manager::unlock_mouse(input_manager* input_manager_context)
{
	glfwSetInputMode(input_manager_context->window->raw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
