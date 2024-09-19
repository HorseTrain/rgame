#pragma once

#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include "chunk_store.h"
#include "thread_manager.h"

struct world;

struct chunk_manager
{
	world*			world_context;
	chunk_store		chunks;
	int				render_distance;

	uint64_t		time;

	thread_manager	data_creation_thread;
	thread_manager	mesh_creation_thread;

	static void		create(chunk_manager* chunk_manager_context, world* world_context);
	static void		destroy(chunk_manager* chunk_manager_context);
	static void		update(chunk_manager* chunk_manager_context);
};

#endif