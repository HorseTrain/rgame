#pragma once

#include <string>

#ifndef LURK_CLIENT_H
#define LURK_CLIENT_H

struct lurk_client
{
	int port;
	int socket_handle;

	static void create(lurk_client* result, int port,std::string name);
	static void destroy(lurk_client* lurk_client_context);

	static bool create_client_socket(int* result_socket, int port, std::string name);

	static void send_data_raw(lurk_client* lurk_client_context, void* data, int count);
	static bool read_data_raw(void* result, int intended_size, lurk_client* lurk_client_context);

	static void send_leave(lurk_client* lurk_client_context);
};

#endif