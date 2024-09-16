#pragma once

#ifndef CHUNK_STORE
#define CHUNK_STORE

#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include "rgame/glm/vec3.hpp"
#include <vector>
#include <mutex>

struct chunk;
struct chunk_manager;

#define KEY_TYPE glm::ivec3

template <>
struct std::hash<KEY_TYPE>
{
	std::size_t operator()(const KEY_TYPE& k) const
	{
		int* buffer = (int*)&k;

		assert(sizeof(KEY_TYPE) % 4 == 0);

		uint64_t working_result = 0;

		for (int i = 0; i < sizeof(KEY_TYPE) / 4; ++i)
		{
			working_result ^= std::hash<int>()(buffer[i]);
		}

		return working_result;
	}
};

struct chunk_store
{
	chunk_manager*							chunk_manager_context;

	std::unordered_map<KEY_TYPE, chunk*>	chunks;
	std::unordered_set<chunk*>				chunk_data_creation_que;
	std::unordered_set<chunk*>				ghosted_chunks;

	std::unordered_set<chunk*>				main_thread_destroyed_chunks;

	std::mutex								chunk_access_lock;
	std::mutex								creation_que_access_lock;
	std::mutex								ghost_chunks_lock;

	bool									chunk_destruction_in_progreass;

	static void								create(chunk_store* chunk_store_context, chunk_manager* chunk_manager_context);
	static void								destroy(chunk_store* chunk_store_context);
	static chunk*							create_or_get_chunk(chunk_store* chunk_store_context, KEY_TYPE location);
	static void								ghost_chunk(chunk_store* chunk_store_context, chunk* chunk_context);
	static void								destroy_ghost_chunks(chunk_store* chunk_store_context);
	static void								append_chunk_to_creation_que(chunk_store* chunk_store_context, chunk* chunk_context);
	static void								remove_chunk_from_creation_que(chunk_store* chunk_store_context, chunk* chunk_context);
	static std::vector<chunk*>				get_vector_from_creation_que(chunk_store* chunk_store_context, bool clear_que = true);
	static bool								creation_que_is_empty(chunk_store* chunk_store_context);
	static chunk*							request_chunk(chunk_store* chunk_store_context, glm::ivec3 position);
};

#endif