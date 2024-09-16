#pragma once

#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

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

#endif