#pragma once


#include "../../ExitCodes.h"

#include "../../../include/minhook/MinHook.h"

#include <string>
#include <Windows.h>

namespace Sky
{
	namespace Core
	{

		class Detours
		{
		private:
			Detours() = delete;
			~Detours() = delete;
		public:
			static MH_STATUS Init();
			static MH_STATUS Destroy();

			static void Patch(BYTE* destination, BYTE* source, unsigned int size);
			static bool Hook(char* source, char* destination, int length);
			static char* TrampHook(char* source, char* destination, unsigned int length);

			static MH_STATUS CreateHook(void* target, void* function, void** original);
			static MH_STATUS EnableAllHooks();
			static MH_STATUS EnableHook(void* target);
			static MH_STATUS DisableAllHooks();
			static MH_STATUS DisableHook(void* target);

			static const char* StatusToString(MH_STATUS status);
		};

	}
}