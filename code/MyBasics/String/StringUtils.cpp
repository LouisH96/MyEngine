#include "StringUtils.h"

using namespace MyEngine;

List<std::string> StringUtils::Split(const std::string& string, char delim)
{
	List<std::string> result{};
	unsigned begin{ 0 };

	for (unsigned i = 0; i < string.size();)
	{
		const char c{ string[i] };
		if (c != delim)
		{
			i++;
			continue;
		}

		result.Add(string.substr(begin, i - begin));
		while (++i < string.size() && string[i] == delim) {}
		begin = i++;
	}
	if (begin != string.size())
		result.Add(string.substr(begin, string.size() - begin));
	return result;
}

bool StringUtils::BeginsWith(const std::string& target, const std::string& begin)
{
	if (target.size() < begin.size())
		return false;

	for (unsigned i = 0; i < begin.size(); i++)
		if (target[i] != begin[i])
			return false;
	return true;
}
