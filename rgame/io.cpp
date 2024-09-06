#include "io.h"
#include "string_tools.h"

#include <fstream>

void io_read_entire_file(char** result, uint64_t* result_size, std::string file_path)
{
	std::ifstream stream = std::ifstream();

	stream.open(file_path, std::ios::binary);

	stream.seekg(0,stream.end);

	*result_size = stream.tellg();

	stream.seekg(0,stream.beg);

	*result = (char*)malloc(*result_size);

	stream.read(*result, *result_size);

	stream.close();
}

void io_read_entire_file_lines(std::vector<std::string>* result, std::string file_path)
{
	char* raw_buffer;
	uint64_t raw_buffer_size;

	io_read_entire_file(&raw_buffer, &raw_buffer_size, file_path);

	std::string string_buffer = std::string(raw_buffer, raw_buffer_size);

	free(raw_buffer);

	split_string(result, string_buffer, '\n');
}
