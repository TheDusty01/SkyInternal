#include "Utils.h"

using namespace Sky::Core;

bool Utils::StringEndsWith(std::string const& fullString, std::string const& ending)
{
	if (fullString.length() >= ending.length())
	{
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else
	{
		return false;
	}
}

bool Utils::StringContains(std::string const& fullString, std::string const& subString)
{
	return fullString.find(subString) != std::string::npos;
}
