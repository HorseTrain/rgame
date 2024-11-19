#include "input_manager.h"
#include "render_window.h"

static bool is_mouse(int key)
{
	return key >= GLFW_MOUSE_BUTTON_1 && key <= GLFW_MOUSE_BUTTON_MIDDLE;
}

static void register_key(input_manager* input_manager_context, int key)
{
	std::map<int, press_data>* key_data_ptr = &input_manager_context->requested_keys;

	(*key_data_ptr)[key] = { 0, 0 };
}

void input_manager::destroy(input_manager* input_manager_context)
{

}

void input_manager::create(input_manager* result, render_window* window)
{
	result->window = window;
	result->requested_keys = std::map<int, press_data>();

	for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
	{
		if (i > GLFW_KEY_WORLD_2 && i < GLFW_KEY_ESCAPE)
			continue;

		register_key(result, i);
	}

	for (int i = GLFW_MOUSE_BUTTON_1; i <= GLFW_MOUSE_BUTTON_MIDDLE; ++i)
	{
		register_key(result, i);
	}
}

void input_manager::update(input_manager* input_manager_context)
{
	std::map<int, press_data>* key_data_ptr = &input_manager_context->requested_keys;

	for (auto i = key_data_ptr->begin(); i != key_data_ptr->end(); ++i)
	{
		int key					= i->first;
		press_data* data_ptr	= &i->second;

		data_ptr->last_down		= data_ptr->is_down;

		if (is_mouse(key))
		{
			data_ptr->is_down = glfwGetMouseButton(input_manager_context->window->raw_window, key);
		}
		else
		{
			data_ptr->is_down = glfwGetKey(input_manager_context->window->raw_window, key);
		}
	}

	double mouse[2];
	glfwGetCursorPos(input_manager_context->window->raw_window, &mouse[0], &mouse[1]);

	input_manager_context->mouse_x = mouse[0];
	input_manager_context->mouse_y = mouse[1];
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
	glfwSetInputMode(input_manager_context->window->raw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void input_manager::unlock_mouse(input_manager* input_manager_context)
{
	glfwSetInputMode(input_manager_context->window->raw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
