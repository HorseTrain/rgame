#pragma once

#ifndef R_ASSET
#define R_ASSET

#include <string>
#include <vector>

struct io;

struct r_asset_file
{
	char*	buffer;
	int		file_size;

	int		read_location;

	static void create_from_path(io* io_context, r_asset_file* result, std::string path);
	static void destroy(r_asset_file* r_asset_file_context);
	static void seek(r_asset_file* file, int seek_location);
	static int peek(r_asset_file* file);
	static std::string read_string(r_asset_file* file);
	static void* get_sub_buffer(r_asset_file* file);
	static char* get_location_pointer(r_asset_file* file);
	static int read_pointer(r_asset_file* file);

	template <typename T>
	static T read(r_asset_file* file)
	{
		T result = *(T*)(file->buffer + file->read_location);

		seek(file, file->read_location + sizeof(T));

		return result;
	}

	template <typename T>
	static void read_array(T* result, int count, r_asset_file* file)
	{
		for (int i = 0; i < count; ++i)
		{
			result[i] = read<T>(file);
		}
	}
};

#endif