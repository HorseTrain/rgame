#include "chunk_manager.h"
#include "chunk.h"
#include <assert.h>

KEY_TYPE get_key(int x, int y, int z)
{
	uint16_t str_buffer[] = { x, y, z, x + y + z };

	return *(uint64_t*)str_buffer;

	//return std::string((char*)str_buffer, sizeof(str_buffer));
}

chunk* chunk_store::get_chunk(chunk_store* chunk_store_context, int x, int y, int z)
{
	KEY_TYPE key = get_key(x, y, z);

	if (chunk_store_context->chunks.find(key) == chunk_store_context->chunks.end())
		return nullptr;

	return chunk_store_context->chunks[key];
}

bool chunk_store::chunk_exists(chunk_store* chunk_store_context, int x, int y, int z)
{
	KEY_TYPE key = get_key(x, y, z);

	return chunk_store_context->chunks.find(key) != chunk_store_context->chunks.end();
}

void chunk_store::set_chunk(chunk_store* chunk_store_context, int x, int y, int z, chunk* new_chunk)
{
	KEY_TYPE key = get_key(x, y, z);

	chunk_store_context->chunks[key] = new_chunk;
}

std::map<KEY_TYPE, chunk*>::iterator chunk_store::get_start(chunk_store* store)
{
	return store->chunks.begin();
}

std::map<KEY_TYPE, chunk*>::iterator chunk_store::get_end(chunk_store* store)
{
	return store->chunks.end();
}

void chunk_store::remove_chunk(chunk_store* chunk_store_context, chunk* to_remove)
{
	int x = to_remove->x;
	int y = to_remove->y;
	int z = to_remove->z;

	KEY_TYPE key = get_key(x, y, z);

	chunk_store_context->chunks.erase(key);
}
