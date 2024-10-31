#pragma once

#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

#include "memory_tools.h"

#include <string>
#include <vector>

static void string_split(std::vector<std::string>* result, std::string source, char delimiter)
{
	*result = std::vector<std::string>();

	std::string working_result = "";

	for (int i = 0; i < source.size(); ++i)
	{
		char working_char = source[i];

		if (working_char == delimiter)
		{
			if (working_result.size() == 0)
				continue;

			result->push_back(working_result);

			working_result = "";

			continue;
		}

		working_result += working_char;
	}

	if (working_result.size() == 0)
		return;

	result->push_back(working_result);
}

static bool starts_with(std::string source, std::string check)
{
	if (source.length() < check.length())
		return false;

	return source.substr(0,check.length()) == check;
}

static bool string_check_sub_string(std::string* source, int offset, std::string to_check)
{
	if (offset + to_check.size() >= source->size())
	{
		return false;
	}

	std::string source_sub_string = source->substr(offset, to_check.size());

	return source_sub_string == to_check;
}

static void string_replace(std::string* result, std::string source, std::string to_replace, std::string to_replace_with)
{
	std::string working_result = "";

	int i = 0;

	while (true)
	{
		if (i >= source.size())
			break;

		if (string_check_sub_string(&source, i, to_replace))
		{
			working_result += to_replace_with;

			i += to_replace.size();
		}
		else
		{
			working_result += source[i];

			i++;
		}
	}

	*result = working_result;
}

static bool string_contains(std::string* test, std::string check)
{
	for (int i = 0; i < test->length() - check.length(); ++i)
	{
		if (test->substr(i, check.size()) == check)
			return true;
	}

	return false;
}

static void string_get_directory(std::string* result, std::string source)
{
	std::vector<std::string> string_parts;
	string_replace(result, source, "\\", "/");
	string_split(&string_parts, *result, '/');

	std::string asset_path_tmp = "";

	for (int i = 0; i < string_parts.size() - 1; ++i)
	{
		asset_path_tmp += string_parts[i] + "/";
	}

	*result = asset_path_tmp;
}

static bool is_sub_string(std::string* source, std::string test, int index)
{
	if (index + test.size() >= source->size())
		return false;

	return source->substr(index, test.size()) == test;
}

static bool is_sub_string(char* c_str, int c_str_size, std::string test, int index)
{
	if (index + test.size() >= c_str_size)
		return false;

	return std::string(c_str + index, test.size()) == test;
}

#endif