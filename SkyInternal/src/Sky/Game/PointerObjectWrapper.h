#pragma once

#include "../Core/Memory.h"

#include <stdint.h>

namespace Sky
{
	using namespace Core;

	namespace Game
	{

		class PointerObjectWrapper
		{
		// Fields
		protected:
			uintptr_t m_baseAddress;
		
		// Methods
		protected:
			template<class T>
			T ReadMemory(uintptr_t offset) const
			{
				return Memory::Read<T>(m_baseAddress + offset);
			}

		public:
			PointerObjectWrapper() = default;
			PointerObjectWrapper(ptrdiff_t offset);
			PointerObjectWrapper(const PointerObjectWrapper&) = default;
			~PointerObjectWrapper() = default;

			bool IsAddressValid() const;
			uintptr_t GetMemoryAddress() const;
		};

	}
}