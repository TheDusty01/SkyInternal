#pragma once

#include "../../ExitCodes.h"
#include "Engine.h"
#include "ImGuiHelper.h"
#include "DirectXHelper.h"
#include "Detours.h"
#include "Visuals.h"

#if _DEBUG
#include "Debug.h"
#endif

#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

// ImGui
#include "../../../include/imgui/imgui.h"
#include "../../../include/imgui/imgui_impl_dx9.h"
#include "../../../include/imgui/imgui_impl_win32.h"


namespace Sky
{
	namespace Core
	{

		// Forward declartion
		class Engine;
		class DirectXHelper;

		//typedef LRESULT(CALLBACK* WndProcFunction)(HWND, UINT, WPARAM, LPARAM);
		using WndProcFunction = WNDPROC;

		class Renderer
		{
			friend class DirectXHelper;
			friend class Engine;

		// Fields
		private:
#if _DEBUG
			const std::shared_ptr<Logger> const m_logger = Logger::Create(TYPE_NAME(Renderer));
#endif
			Engine& m_engine;
			DirectXHelper* m_directXhelper;
			ImGuiHelper* m_imGuiHelper;
			Visuals* m_visuals;

			// Window
			HWND m_window, m_d3dWindow;
			int32_t m_windowWidth, m_windowHeight;
			static WndProcFunction s_oWndProcFunction;
		public:
			bool m_showMenu;

		// Methods
		private:
			HRESULT InitWindow();
			static BOOL __stdcall EnumWindowsCallback(HWND handle, LPARAM lParam);
			static LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			// Events
			void OnRender(IDirect3DDevice9* device);
			void OnDeviceLost();
			void OnDeviceReset();
		public:
			Renderer(Engine& engine);
			Renderer(const Renderer&) = delete;
			Renderer& operator=(const Renderer&) = delete;
			~Renderer();

			HRESULT Create();
		};
	
	}
}