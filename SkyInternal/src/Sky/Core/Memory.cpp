#include "Memory.h"

#include "../../ExitCodes.h"

using namespace Sky::Core;

uintptr_t Memory::s_clientAddress = 0;
uintptr_t Memory::s_engineAddress = 0;

HRESULT Memory::LoadModules()
{
	// TODO: Should be considered for the future: https://github.com/Speedi13/Custom-GetProcAddress-and-GetModuleHandle-and-more
	// or
	// https://daaxr.blogspot.com/2016/12/custom-getprocaddress-and.html?m=1

	// Get client address
	s_clientAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(L"client.dll"));
	if (s_clientAddress == 0)
	{
		return SKY_ERROR_GET_CLIENT;
	}

	// Get engine address
	s_engineAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(L"engine.dll"));
	if (s_engineAddress == 0)
	{
		return SKY_ERROR_GET_ENGINE;
	}

	return SKY_SUCCESS;
}
