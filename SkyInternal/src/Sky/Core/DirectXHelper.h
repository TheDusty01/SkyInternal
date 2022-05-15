#pragma once

#include "../../ExitCodes.h"
#include "Renderer.h"

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

namespace Sky
{
	namespace Core
	{

		// Forward declartion
		class Renderer;

		// Function pointer types
		using ResetFunction = HRESULT(APIENTRY*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		using EndSceneFunction = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice);

		class DirectXHelper
		{
			// Fields
		private:
#if _DEBUG
			const std::shared_ptr<Logger> const m_logger = Logger::Create(TYPE_NAME(DirectXHelper));
#endif
			HWND m_window;

			// D3D9Device vTable
			void* m_d3d9DeviceVTable[119];

			// Original methods
			static ResetFunction s_oResetFunction;
			static EndSceneFunction s_oEndSceneFunction;

			// Methods
		private:
			// Hooked methods
			static HRESULT __stdcall OnReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentationParameters);
			static HRESULT __stdcall OnEndScene(IDirect3DDevice9* device);

			HRESULT InitD3D9Device();
			HRESULT HookMethods();
			void UnhookMethods();
		public:
			DirectXHelper(HWND window);
			~DirectXHelper();

			HRESULT Create();
		};

	}
}