#pragma once

#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

#include <string>
#include <vector>

static void split_string(std::vector<std::string>* result, std::string source, char delimiter)
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
#endif