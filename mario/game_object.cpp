#include "game_object.h"
#include "mario.h"
#include "rgame/render_window.h"

void game_object::create(game_object** result, std::string name, level* level_context)
{
	if (level_context == nullptr)
	{
		assert(false);

		throw 0;
	}

	game_object* result_object = lifetime_memory_manager::allocate<game_object>(&level_context->memory, name);

	result_object->render_window_context = level_context->game_context->window_context;
	result_object->level_context = level_context;
	result_object->game_context = level_context->game_context;
	result_object->name = name;

	result_object->marked_for_destruction = false;
	result_object->started = false;

	level::append_to_que_with_lock(result_object->level_context, &result_object->level_context->game_object_creation_que, result_object);

	result_object->game_object_data = nullptr;
	
	result_object->object_data_start = nullptr;
	result_object->object_data_update = nullptr;

	lifetime_memory_manager::create(&result_object->memory);

	*result = result_object;
}

void game_object::start(game_object* game_object_context)
{
	if (game_object_context->object_data_start == nullptr || game_object_context->game_object_data == nullptr)
		return;

	((void (*)(void*))game_object_context->object_data_start)(game_object_context->game_object_data);
}

void game_object::update(game_object* game_object_context)
{
	if (game_object_context->object_data_update == nullptr || game_object_context->game_object_data == nullptr)
		return;

	((void (*)(void*))game_object_context->object_data_update)(game_object_context->game_object_data);
}

void game_object::destroy(game_object* game_object_context)
{
	game_object_context->marked_for_destruction = true;

	lifetime_memory_manager::destroy(&game_object_context->memory);

	if (game_object_context->level_context->level_is_closing)
		return;

	level::append_to_que_with_lock(game_object_context->level_context, &game_object_context->level_context->game_object_destruction_que, game_object_context);

	if (game_object_context->game_object_data != nullptr)
	{
		lifetime_memory_manager::delete_buffer(&game_object_context->level_context->memory, game_object_context->game_object_data);
	}
}
