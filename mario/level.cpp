#include "level.h"

#include "mario.h"

static void create_game_objects(level* level_context)
{
	level_context->game_object_accessor_lock.lock();

	for (auto i : level_context->game_object_creation_que)
	{
		if (level_context->active_game_objects.find(i) != level_context->active_game_objects.end())
		{
			assert(false);

			throw 0;
		}

		level_context->active_game_objects.insert(i);
	}

	level_context->game_object_creation_que = std::unordered_set<game_object*>();

	level_context->game_object_accessor_lock.unlock();
}

static void destroy_game_objects(level* level_context)
{
	level_context->game_object_accessor_lock.lock();

	for (auto i : level_context->game_object_destruction_que)
	{
		level_context->active_game_objects.erase(i);
		level_context->game_object_creation_que.erase(i);
	}

	level_context->game_object_destruction_que = std::unordered_set<game_object*>();

	level_context->game_object_accessor_lock.unlock();
}

static void update_game_objects(level* level_context)
{
	for (auto i : level_context->active_game_objects)
	{
		game_object* working_object = i;

		if (working_object->marked_for_destruction)
			continue;

		if (!working_object->started)
		{
			game_object::start(working_object);

			working_object->started = true;
		}

		game_object::update(working_object);
	}
}

void level::create(level* result, game* game_context)
{
	result->game_context = game_context;
	lifetime_memory_manager::create(&result->memory);

	result->level_is_closing = false;
}

void level::append_to_que_with_lock(level* level_context, std::unordered_set<game_object*>* que, game_object* object)
{
	level_context->game_object_accessor_lock.lock();

	if (que->find(object) != que->end())
	{
		assert(false);

		throw 0;
	}

	que->insert(object);

	level_context->game_object_accessor_lock.unlock();
}

void level::update(level* level_context)
{
	create_game_objects(level_context);

	update_game_objects(level_context);

	destroy_game_objects(level_context);
}

void level::destroy(level* level_context)
{
	level_context->level_is_closing = true;

	lifetime_memory_manager::destroy(&level_context->memory);
}
