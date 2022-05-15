#include "PointerObjectWrapper.h"

using namespace Sky::Game;

PointerObjectWrapper::PointerObjectWrapper(ptrdiff_t offset)
	: m_baseAddress(*reinterpret_cast<uintptr_t*>(offset))
{
}

bool PointerObjectWrapper::IsAddressValid() const
{
	return m_baseAddress != 0;
}

uintptr_t PointerObjectWrapper::GetMemoryAddress() const
{
	return m_baseAddress;
}
