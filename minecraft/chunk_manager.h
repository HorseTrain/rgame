#pragma once

#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include "chunk_store.h"
#include "thread_manager.h"
#include "perlin.h"

struct world;

struct chunk_manager
{
	world*			world_context;
	chunk_store		chunks;
	int				render_distance;

	perlin			normal_noise;
	perlin			mountain_noise;
	perlin			mini_noise;

	uint64_t		time;

	thread_manager	data_creation_thread;
	thread_manager	mesh_creation_thread;

	static void		create(chunk_manager* chunk_manager_context, world* world_context);
	static void		destroy(chunk_manager* chunk_manager_context);
	static void		update(chunk_manager* chunk_manager_context);
	static void		ready_chunk_for_destruction(chunk_manager* chunk_manager_context, chunk* chunk_context);
};

#endif