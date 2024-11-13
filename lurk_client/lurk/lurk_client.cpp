#include "lurk_client.h"

#include "WinSock2.h"
#include "winsock.h"
#include "stdio.h"
#include <iostream>

bool wsa_open = false;

void lurk_client::create(lurk_client* result,int port, std::string name)
{
	result->port = port;

	while (!create_client_socket(&result->socket_handle, port, name))
	{
		result->port++;
	}

	std::cout << "Connected to port " << result->port << std::endl;
}

void lurk_client::destroy(lurk_client* lurk_client_context)
{
	if (lurk_client_context->port == -1)
		return;

	closesocket(lurk_client_context->socket_handle);
}

bool lurk_client::create_client_socket(int* result_socket, int port, std::string name)
{
	if (!wsa_open)
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

		/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
		wVersionRequested = MAKEWORD(2, 2);

		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0) {
			/* Tell the user that we could not find a usable */
			/* Winsock DLL.                                  */
			printf("WSAStartup failed with error: %d\n", err);
			return false;
		}

		wsa_open = true;
	}

	struct sockaddr_in sad;
	sad.sin_port = htons(port);
	sad.sin_family = AF_INET;

	int skt = socket(AF_INET, SOCK_STREAM, 0);

	// do a dns lookup
	struct hostent* entry = gethostbyname(name.c_str());

	int error = WSAGetLastError();

	if (!entry) {
		if (h_errno == HOST_NOT_FOUND) {
			printf("This is our own message that says the host wasn't found\n");
		}
		printf("gethostbyname");
		return 0;
	}
	struct in_addr** addr_list = (struct in_addr**)entry->h_addr_list;
	struct in_addr* c_addr = addr_list[0];
	char* ip_string = inet_ntoa(*c_addr);
	sad.sin_addr = *c_addr; // copy the address we found into sad
	// Finally done with DNS!
	printf("Connecting to:  %s\n", ip_string);

	if (connect(skt, (struct sockaddr*)&sad, sizeof(struct sockaddr_in))) {
		printf("connect");
		return 0;
	}

	*result_socket = skt;

	return 1;
}

void lurk_client::send_data_raw(lurk_client* lurk_client_context, void* data, int count)
{
	send(lurk_client_context->socket_handle, (const char*)data, count, 0);
}

bool lurk_client::read_data_raw(void* result, int intended_size, lurk_client* lurk_client_context)
{
	return recv(lurk_client_context->socket_handle, (char*)result, intended_size, MSG_WAITALL) == intended_size;
}


