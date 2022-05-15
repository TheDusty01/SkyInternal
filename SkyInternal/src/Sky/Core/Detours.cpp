#include "Detours.h"

using namespace Sky::Core;

MH_STATUS Detours::Init()
{
	return MH_Initialize();
}

MH_STATUS Detours::Destroy()
{
	return MH_Uninitialize();
}

void Detours::Patch(BYTE* destination, BYTE* source, unsigned int size)
{
	DWORD oldProc;
	VirtualProtect(destination, size, PAGE_EXECUTE_READWRITE, &oldProc);
	memcpy(destination, source, size);
	VirtualProtect(destination, size, oldProc, &oldProc);
}

bool Detours::Hook(char* source, char* destination, int length)
{
	if (length < 5)
		return false;

	DWORD oldProc;
	VirtualProtect(source, length, PAGE_EXECUTE_READWRITE, &oldProc);
	memset(source, 0x90, length);
	uintptr_t relAddress = (uintptr_t)(destination - source - 5);
	*source = (char)0xE9;
	*(uintptr_t*)(source + 1) = (uintptr_t)relAddress;
	VirtualProtect(source, length, oldProc, &oldProc);

	return true;
}

char* Detours::TrampHook(char* source, char* destination, unsigned int length)
{
	if (length < 5)
		return 0;

	char* gateway = (char*)VirtualAlloc(0, length + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(gateway, source, length);
	uintptr_t jumpAddress = (uintptr_t)(source - gateway - 5);
	*(gateway + length) = char(0xE9);
	*(uintptr_t*)(gateway + length + 1) = jumpAddress;

	if (Hook(source, destination, length))
	{
		return gateway;
	}
	else
	{
		return nullptr;
	}
}

MH_STATUS Detours::CreateHook(void* target, void* function, void** original)
{
	return MH_CreateHook(target, function, original);
}

MH_STATUS Detours::EnableAllHooks()
{
	return EnableHook(MH_ALL_HOOKS);
}

MH_STATUS Detours::EnableHook(void* target)
{
	return MH_EnableHook(target);
}

MH_STATUS Detours::DisableAllHooks()
{
	return DisableHook(MH_ALL_HOOKS);
}

MH_STATUS Detours::DisableHook(void* target)
{
	return MH_DisableHook(target);
}

const char* Detours::StatusToString(MH_STATUS status)
{
	return MH_StatusToString(status);
}
