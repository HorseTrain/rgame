#pragma once

#ifndef IO_H
#define IO_H

#include <iostream>
#include <string>
#include <vector>

struct io
{
	std::string			asset_path;

	static void			create(io* io_context, std::string asset_path);
	static std::string	get_path(io* io_context, std::string asset_path);
	static void			io_read_entire_file(io* io_context, char** result, uint64_t* result_size, std::string file_path);
	static void			io_read_entire_file_lines(io* io_context, std::vector<std::string>* result, std::string file_path);
	static void			get_parent_directory(std::string* result,std::string path);
	static void			get_file_name(std::string* result, std::string path);
	static void			fix_path(std::string* result, std::string source);
	static bool			file_exists(std::string path);
	static bool			file_exists(io* io_context,std::string path);
};

#endif