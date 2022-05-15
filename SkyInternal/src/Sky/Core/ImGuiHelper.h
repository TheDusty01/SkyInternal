#pragma once

#include "../../../include/imgui/imgui.h"
#include "../../../include/imgui/imgui_impl_dx9.h"
#include "../../../include/imgui/imgui_impl_win32.h"

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace Sky
{
	namespace Core
	{

		class ImGuiHelper
		{
		private:
			bool m_isReady;
		public:
			ImGuiHelper(HWND window, IDirect3DDevice9* device);
			~ImGuiHelper();

			void OnDeviceLost();
			void OnDeviceReset();

			void NewFrame();
			void EndFrame();

			static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		};

	}
}