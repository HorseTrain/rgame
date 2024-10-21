#include "io.h"
#include "string_tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include <iostream>

void io::io_read_entire_file(io* io_context, char** result, uint64_t* result_size, std::string file_path)
{
	file_path = io::get_path(io_context, file_path);

	std::ifstream stream = std::ifstream();

	stream.open(file_path, std::ios::binary);

	stream.seekg(0, stream.end);

	*result_size = stream.tellg();

	stream.seekg(0, stream.beg);

	*result = (char*)malloc(*result_size);

	stream.read(*result, *result_size);

	stream.close();
}

void io::io_read_entire_file_lines(io* io_context, std::vector<std::string>* result, std::string file_path)
{
	file_path = io::get_path(io_context, file_path);

	char* raw_buffer;
	uint64_t raw_buffer_size;

	io_read_entire_file(io_context, &raw_buffer, &raw_buffer_size, file_path);

	std::string string_buffer = std::string(raw_buffer, raw_buffer_size);

	free(raw_buffer);

	string_split(result, string_buffer, '\n');
}

void io::get_parent_directory(std::string* result, std::string path)
{
	fix_path(&path, path);

	std::vector<std::string> path_parts;
	string_split(&path_parts, path, '\\');

	*result = "";

	for (int i = 0; i < path_parts.size() - 1; ++i)
	{
		*result += path_parts[i] + "\\";
	}
}

void io::get_file_name(std::string* result, std::string path)
{
	fix_path(&path, path);

	std::vector<std::string> path_parts;
	string_split(&path_parts, path, '/');

	*result = path_parts[path_parts.size() - 1];
}

void io::fix_path(std::string* result, std::string source)
{
	*result = source;

	while (true)
	{
		int length = result->length();

		string_replace(result, *result, "/", "\\");
		string_replace(result, *result, "\\\\", "\\");

		if (length == result->length())
			break;
	}
}

bool io::file_exists(std::string path)
{
	std::ifstream to_check = std::ifstream();

	fix_path(&path, path);

	to_check.open(path.c_str(), std::ios::binary);

	bool result;

	if (to_check.fail())
	{
		result = false;
	}
	else
	{
		result = true;

		to_check.close();
	}

	return result;
}

void io::destroy(io* to_destroy)
{
	//
}

bool io::file_exists(io* io_context, std::string path)
{
	path = get_path(io_context, path);

	return file_exists(path);
}

void io::create(io* io_context, std::string asset_path)
{
	io_context->asset_path = asset_path;
}

std::string io::get_path(io* io_context, std::string asset_path)
{
	std::string working_path;

	if (string_contains(&asset_path, ":"))
	{
		working_path = asset_path;
	}
	else
	{
		working_path = io_context->asset_path + "\\" + asset_path;
	}

	fix_path(&working_path, working_path);

	return working_path;
}
