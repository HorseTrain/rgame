#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <map>
#include <vector>
#include <string>
#include <thread>
#include "mutex"

#include <iostream>
#include "rgame/glm/vec3.hpp"
#include "rgame/big_int.h"

struct world;
struct chunk;

#define KEY_TYPE uint64_t

struct chunk_store
{
	std::map<KEY_TYPE, chunk*>	chunks;
	std::vector<chunk*>			chunk_creation_que;
	std::vector<chunk*>			chunk_destruction_que;

	static chunk* get_chunk(chunk_store* chunk_store_context, int x, int y, int z);
	static bool chunk_exists(chunk_store* chunk_store_context, int x, int y, int z);
	static void set_chunk(chunk_store* chunk_store_context, int x, int y, int z, chunk* new_chunk);
	static std::map<KEY_TYPE, chunk*>::iterator get_start(chunk_store* store);
	static std::map<KEY_TYPE, chunk*>::iterator get_end(chunk_store* store);
	static void remove_chunk(chunk_store* chunk_store_context, chunk* to_remove);
};

struct chunk_manager
{
	world*										world_context;
	chunk_store									chunks;

	std::thread									chunk_generation_thread;

	static void			create(chunk_manager* result, world* world_context);
	static void			destroy(chunk_manager* to_destroy);
	static void			request_chunk_for_creation(chunk_manager* chunk_manager_context, int x, int y, int z);
	static void			generate_chunk_data_in_que(chunk_manager* chunk_manager_context);
	static void			destroy_chunk_data_in_que(chunk_manager* chunk_manager_context);
	static void			update_existing_chunks(chunk_manager* chunk_manager_context);
	static void			chunk_manager_update(chunk_manager* chunk_manager_context);
	static void			insert_chunk_into_data_generation_que(chunk_manager* chunk_manager_context, chunk* chunk_context);
	static void			insert_chunk_into_data_destruction_que(chunk_manager* chunk_manager_context, chunk* chunk_context);
	static chunk*		get_chunk(chunk_manager* chunk_manager_context, int x, int y, int z);
	static void			generate_neighbor_chunks(chunk_manager* chunk_manager_context, chunk* chunk_context);

	static std::string	get_chunk_key(int x, int y, int z);
};

#endif