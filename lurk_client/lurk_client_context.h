#pragma once

#include <string>
#include <vector>

#ifndef LURK_CLIENT_CONTEXT_H
#define LURK_CLIENT_CONTEXT_H

struct lurk_client;

struct game_data
{
	int			initial_points;
	int			stat_min;
	int			stat_max;

	std::string description;
};

struct lurk_client_context
{
	lurk_client*		raw_client;
	std::vector<int>	version;

	static void create(lurk_client_context* result, int port, std::string name);
	static void destroy(lurk_client_context* lurk_client_context_context);

	static void run(lurk_client_context* lcc);
};

#endif