#include "lurk_client_context.h"
#include "lurk/lurk_client.h"
#include <assert.h>

#include "lurk/lurk_structs.h"

static void read_lurk_version(lurk_client_context* lcc)
{
	lurk_client* raw_client = lcc->raw_client;

	lurk_version version_header;

	lurk_client::read_data_raw(&version_header, sizeof(lurk_version), raw_client);

	lcc->version = std::vector<int>({ version_header.major, version_header.minor });

	int minor_count = version_header.extension_list;

	assert(minor_count == 0);

	if (minor_count != 0)
	{
		throw 0;
	}
}

static void read_lurk_description(lurk_client_context* lcc)
{
	lurk_client* raw_client = lcc->raw_client;

	lurk_game game_header;

	lurk_client::read_data_raw(&game_header, sizeof(lurk_game), raw_client);

	char* message_buffer = (char*)malloc(game_header.description_length);

	lurk_client::read_data_raw(message_buffer, game_header.description_length, raw_client);

	free(message_buffer);
}

void lurk_client_context::create(lurk_client_context* result, int port, std::string name)
{
	result->raw_client = new lurk_client();

	lurk_client::create(result->raw_client, port, name);
}

void lurk_client_context::destroy(lurk_client_context* lurk_client_context_context)
{
	lurk_client::destroy(lurk_client_context_context->raw_client);

	delete lurk_client_context_context->raw_client;
}

void lurk_client_context::run(lurk_client_context* lcc)
{
	read_lurk_version(lcc);

	read_lurk_description(lcc);
}
