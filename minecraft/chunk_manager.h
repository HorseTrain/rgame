#pragma once

#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

struct world;

struct chunk_manager
{
	world* world_context;

	static void create(chunk_manager* chunk_manager_context, world* world_context);
	static void destroy(chunk_manager* chunk_manager_context);
};

#endif