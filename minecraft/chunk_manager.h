#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <map>
#include <vector>
#include <string>
#include <thread>
#include "mutex"

struct world;
struct chunk;

struct chunk_manager
{
	world*							world_context;
	std::map<std::string, chunk*>	chunks;
	std::vector<chunk*>				chunk_creation_que;
	std::mutex						chunk_lock;

	std::thread						chunk_generation_thread;

	static void			create(chunk_manager* result, world* world_context);
	static void			destroy(chunk_manager* to_destroy);
	static void			request_chunk_for_creation(chunk_manager* chunk_manager_context, int x, int y, int z);
	static void			generate_chunk_data_in_que(chunk_manager* chunk_manager_context);
	static void			update_existing_chunks(chunk_manager* chunk_manager_context);
	static void			chunk_manager_update(chunk_manager* chunk_manager_context);
	static void			insert_chunk_into_data_generation_que(chunk_manager* chunk_manager_context, chunk* chunk_context);
	static chunk*		get_chunk(chunk_manager* chunk_manager_context, int x, int y, int z);

	static std::string	get_chunk_key(int x, int y, int z);
};

#endif