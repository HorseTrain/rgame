#include "r_asset.h"

#include "io.h"

void r_asset_file::create_from_path(io* io_context, r_asset_file* result, std::string path)
{
	char* file_buffer;
	uint64_t file_size;

	io::io_read_entire_file(io_context, &file_buffer, &file_size, path);

	result->buffer = file_buffer;
	result->file_size = file_size;

	seek(result, 0);
}

void r_asset_file::destroy(r_asset_file* r_asset_file_context)
{
	free(r_asset_file_context->buffer);
}

void r_asset_file::seek(r_asset_file* file, int seek_location)
{
	file->read_location = seek_location;
}

int r_asset_file::peek(r_asset_file* file)
{
	return file->read_location;
}

std::string r_asset_file::read_string(r_asset_file* file)
{
	char* data_buffer = (char*)get_sub_buffer(file);

	int string_size = *(int*)data_buffer;

	std::string result = std::string(data_buffer + 4, string_size);

	return result;
}

void* r_asset_file::get_sub_buffer(r_asset_file* file)
{
	int data_location = read_pointer(file);
	int return_location = peek(file);

	seek(file, data_location);

	void* result = get_location_pointer(file);

	seek(file, return_location);

	return result;
}

char* r_asset_file::get_location_pointer(r_asset_file* file)
{
	return file->buffer + peek(file);
}

int r_asset_file::read_pointer(r_asset_file* file)
{
	int start_location = peek(file);
	int offset = read<int>(file);

	return start_location + offset;
}