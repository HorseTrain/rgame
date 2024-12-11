#include "chunk_store.h"
#include "chunk.h"

#include <functional>
#include <iostream>

void chunk_store::create(chunk_store* chunk_store_context, chunk_manager* chunk_manager_context)
{
	chunk_store_context->chunk_manager_context = chunk_manager_context;
	chunk_store_context->chunks = std::unordered_map<KEY_TYPE, chunk*>();

	chunk_store_context->chunk_destruction_in_progreass = false;
}

void chunk_store::destroy(chunk_store* chunk_store_context)
{
	for (auto i = chunk_store_context->chunks.begin(); i != chunk_store_context->chunks.end(); ++i)
	{
		chunk* working_chunk = i->second;

		chunk::destroy(working_chunk);
	}
}

chunk* chunk_store::create_or_get_chunk(chunk_store* chunk_store_context, KEY_TYPE location)
{
	for (int i = 0; i < 3; ++i)
	{
		assert(location[i] % CUBE_CHUNK_SIZE == 0);
	}

	std::mutex* lock = &chunk_store_context->chunk_access_lock;
	std::unordered_map<KEY_TYPE, chunk*>* chunks = &chunk_store_context->chunks;

	lock->lock();

	if (chunks->find(location) != chunks->end())
	{
		chunk* result = (*chunks)[location];

		lock->unlock();

		return result;
	}

	chunk* new_chunk;
	chunk::create(&new_chunk, chunk_store_context->chunk_manager_context, location);

	(*chunks)[location] = new_chunk;

	lock->unlock();

	return new_chunk;
}

void chunk_store::ghost_chunk(chunk_store* chunk_store_context, chunk* chunk_context, uint64_t* times)
{
	std::mutex* lock = &chunk_store_context->ghost_chunks_lock;

	std::unordered_map<KEY_TYPE, chunk*>* all_chunks = &chunk_store_context->chunks;
	std::unordered_set<chunk*>* ghosted_chunks = &chunk_store_context->ghosted_chunks;

	chunk_context->marked_for_destruction = true;

	lock->lock();

	remove_chunk_from_creation_que(chunk_store_context, chunk_context);

	KEY_TYPE location = chunk_context->position;

	all_chunks->erase(location);
	ghosted_chunks->insert(chunk_context);

	memcpy(chunk_context->destruction_times, times, sizeof(uint64_t) * 3);

	lock->unlock();
}

void chunk_store::destroy_ghost_chunks(chunk_store* chunk_store_context)
{
	std::mutex* lock = &chunk_store_context->ghost_chunks_lock;

	std::unordered_set<chunk*>* ghosted_chunks = &chunk_store_context->ghosted_chunks;

	lock->lock();

	for (auto i = ghosted_chunks->begin(); i != ghosted_chunks->end(); ++i)
	{

	}

	lock->unlock();
}

void chunk_store::append_chunk_to_creation_que(chunk_store* chunk_store_context, chunk* chunk_context)
{
	std::mutex* lock = &chunk_store_context->creation_que_access_lock;
	std::unordered_set<chunk*>* chunks = &chunk_store_context->chunk_data_creation_que;

	if (chunk_context->mesh_ready && chunk_context->data_ready)
		return;

	lock->lock();

	if (chunks->find(chunk_context) == chunks->end())
	{
		chunks->insert(chunk_context);
	}

	lock->unlock();
}

void chunk_store::remove_chunk_from_creation_que(chunk_store* chunk_store_context, chunk* chunk_context)
{
	std::mutex* lock = &chunk_store_context->creation_que_access_lock;
	std::unordered_set<chunk*>* chunks = &chunk_store_context->chunk_data_creation_que;

	lock->lock();

	chunks->erase(chunk_context);

	lock->unlock();
}

std::vector<chunk*> chunk_store::get_vector_from_creation_que(chunk_store* chunk_store_context, bool clear_que)
{
	std::vector<chunk*> result;

	std::mutex* lock = &chunk_store_context->creation_que_access_lock;
	std::unordered_set<chunk*>* chunks = &chunk_store_context->chunk_data_creation_que;

	lock->lock();

	for (auto i = chunks->begin(); i != chunks->end(); ++i)
	{
		result.push_back(*i);
	}

	if (clear_que)
		*chunks = std::unordered_set<chunk*>();

	lock->unlock();

	return result;
}

bool chunk_store::creation_que_is_empty(chunk_store* chunk_store_context)
{
	std::mutex* lock = &chunk_store_context->creation_que_access_lock;

	bool result;

	lock->lock();

	result = chunk_store_context->chunk_data_creation_que.size() == 0;

	lock->unlock();

	return result;
}

chunk* chunk_store::request_chunk(chunk_store* chunk_store_context, glm::ivec3 position)
{
	std::mutex* lock = &chunk_store_context->chunk_access_lock;
	std::unordered_map<KEY_TYPE, chunk*>* chunks = &chunk_store_context->chunks;

	if (chunks->find(position) == chunks->end())
	{
		return nullptr;
	}

	lock->lock();

	chunk* result = (*chunks)[position];

	lock->unlock();

	return result;
}