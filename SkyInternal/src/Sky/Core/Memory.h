#pragma once

#include "../Game/Offsets.h"

#include <Windows.h>
#include <stdint.h>

namespace Sky
{
	namespace Core
	{

		class Memory
		{
		// Fields
		public:
			static uintptr_t s_clientAddress;
			static uintptr_t s_engineAddress;

		// Methods
		public:
			static HRESULT LoadModules();

			// @brief Get the value of an address in memory without casting mistakes.
			// @tparam T - Type of the address
			// @param address - Unsigned integer of the address
			// @returns Value (casted to T) of the address
			template <class T>
			static T Read(uintptr_t address)
			{
				return *(reinterpret_cast<T*>(address));
			}

			template <class T>
			static T ReadClient(uintptr_t address)
			{
				return Read<T>(s_clientAddress + address);
			}


			template <class T>
			static T ReadEngine(uintptr_t address)
			{
				return Read<T>(s_engineAddress + address);
			}

			// @brief Get the value of an address in memory without casting mistakes.
			// @tparam T - Type of the address
			// @param address - Unsigned integer of the address
			// @param outValue - Reference to the object where the value (casted to T) of the address should be stored
			template <class T>
			static void Read(uintptr_t address, T& outValue)
			{
				outValue = *(reinterpret_cast<T*>(address));
			}

			template <class T>
			static void ReadClient(uintptr_t address, T& outValue)
			{
				Read(s_clientAddress + address, outValue);
			}

			template <class T>
			constexpr static void ReadEngine(uintptr_t address, T& outValue)
			{
				Read(s_engineAddress + address, outValue);
			}

			// @brief Set the value of an address in memory without casting mistakes.
			// @tparam T - Type of the address
			// @param address - Unsigned integer of the address
			// @param value - Value to set
			template <class T>
			static void Write(uintptr_t address, T value)
			{
				*(T*)address = value;
			}

			// TODO: https://guidedhacking.com/threads/external-internal-pattern-scanning-guide.14112/
			static void PatternScan() {};
		};

	}
}