#pragma once

#include <string>
#include <array>

namespace Sky
{
	namespace Core
	{

		class Utils
		{
		public:
			// Strings
			static bool StringEndsWith(std::string const& fullString, std::string const& ending);
			static bool StringContains(std::string const& fullString, std::string const& subString);

			// Arrays
			template <typename T, T value, size_t length>
			static constexpr std::array<T, length> InitArray()
			{
				std::array<T, length> array;
				//for (size_t i = 0; i < length; i++)
				//	array[i] = value;
				array.fill(value);
				return array;
			}
		};

	}
}
