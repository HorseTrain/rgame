#include "shader_compiler.h"
#include "string_tools.h"
#include "io.h"
#include <vector>

#define ITL intrusive_linked_list<char>

static void shader_compiler_error(std::string path)
{
	std::cout << "Invalid shader file at " << path << std::endl;

	assert(false);

	throw 0;
}

static void compile_shader_source_recursive(ITL* result, std::string path, io* io_context, std::vector<std::string>* taken_paths)
{
	if (!io::file_exists(io_context,path))
	{
		shader_compiler_error(path);
	}

	char* file_buffer;
	uint64_t file_size;

	io::io_read_entire_file(io_context, &file_buffer, &file_size, path);

	for (int i = 0; i < taken_paths->size(); ++i)
	{
		if ((*taken_paths)[i] == path)
		{
			shader_compiler_error(path);
		}
	}

	taken_paths->push_back(path);

	int sl = 0;

	std::string include = "#include";

	while (1)
	{
		if (sl == file_size)
		{
			break;
		}

		if (is_sub_string(file_buffer, file_size, include, sl))
		{
			sl += include.size();

			for (int i = sl; i < file_size; ++i)
			{
				if (i == file_size - 1)
				{
					shader_compiler_error(path);
				}

				if (file_buffer[i] == '\"')
				{
					sl = i + 1;

					break;
				}
			}

			std::string working_path;

			for (int i = sl; i < file_size; ++i)
			{
				if (i == file_size - 1)
				{
					shader_compiler_error(path);
				}

				char working_char = file_buffer[i];

				if (working_char == '\"')
				{
					sl = i + 1;

					break;
				}

				working_path += working_char;
			}

			compile_shader_source_recursive(result, working_path, io_context, taken_paths);
		}
		else
		{
			ITL::insert_element(result, file_buffer[sl]);
			
			++sl;
		}
	}

	free(file_buffer);

	taken_paths->pop_back();
}

void compile_shader_source(std::string* result, std::string path, io* io_context)
{
	arena_allocator working_allocator = arena_allocator::create(1024 * 10);
	std::vector<std::string> read_files;

	ITL* working_result = ITL::create(&working_allocator, 0, 0);

	compile_shader_source_recursive(working_result, path, io_context, &read_files);

	int count = 0;

	for (auto i = working_result->first; i != working_result->last; i = i->next)
	{
		++count;
	}

	char* result_buffer = (char*)malloc(count);

	int idx = 0;
	for (auto i = working_result->first; i != working_result->last; i = i->next)
	{
		if (i->data == 0)
			continue;

		result_buffer[idx] = i->data;

		idx++;
	}

	*result = std::string(result_buffer, idx);

	free(result_buffer);

	arena_allocator::destroy(&working_allocator);
}
