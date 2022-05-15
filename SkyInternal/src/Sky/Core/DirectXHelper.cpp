#include "DirectXHelper.h"

using namespace Sky::Core;

// Static variables
EndSceneFunction DirectXHelper::s_oEndSceneFunction = nullptr;
ResetFunction DirectXHelper::s_oResetFunction = nullptr;

// OnReset hook
HRESULT __stdcall DirectXHelper::OnReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentationParameters)
{
    Renderer& renderer = Engine::Instance().GetRenderer();

    // OnDeviceLost
    renderer.OnDeviceLost();

    HRESULT hr = s_oResetFunction(device, presentationParameters);

    if (hr >= 0)
    {
        // Reset device
        renderer.OnDeviceReset();
    }

    return hr;
}

// OnEndScene hook
HRESULT __stdcall DirectXHelper::OnEndScene(IDirect3DDevice9* device)
{
    Renderer& renderer = Engine::Instance().GetRenderer();

    renderer.OnRender(device);

    return s_oEndSceneFunction(device);
}

// Init
DirectXHelper::DirectXHelper(HWND window) : m_window(window), m_d3d9DeviceVTable()
{
}

DirectXHelper::~DirectXHelper()
{
    UnhookMethods();
    //Detours::Patch((BYTE*)m_d3d9Device[42], m_originalEndSceneBytes, 7);
}

HRESULT DirectXHelper::Create()
{
    // Get d3d9 device
    HRESULT hr = InitD3D9Device();
    if (hr != SKY_SUCCESS)
        return hr;

    // Hook
    hr = HookMethods();
    if (hr != SKY_SUCCESS)
        return hr;

    return SKY_SUCCESS;
}

HRESULT DirectXHelper::InitD3D9Device()
{
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    if (!pD3D)
        return SKY_ERROR_DIRECT3D_CREATE;

    IDirect3DDevice9* pDummyDevice = NULL;

    // options to create dummy device
    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = false;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = m_window;

    HRESULT dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

    if (dummyDeviceCreated != S_OK)
    {
        // may fail in windowed fullscreen mode, trying again with windowed mode
        d3dpp.Windowed = !d3dpp.Windowed;

        dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

        if (dummyDeviceCreated != S_OK)
        {
            pD3D->Release();
            return SKY_ERROR_DIRECT3D_CREATE_DEVICE;
        }
    }

    memcpy(m_d3d9DeviceVTable, *reinterpret_cast<void***>(pDummyDevice), sizeof(m_d3d9DeviceVTable));

    pDummyDevice->Release();
    pD3D->Release();

    return SKY_SUCCESS;
}

HRESULT DirectXHelper::HookMethods()
{
    // Init detour context
    MH_STATUS status = Detours::Init();
    if (status != MH_OK)
    {
#if _DEBUG
        m_logger->LogDebug("Failed to initialze detours: %s", Detours::StatusToString(status));
#endif
        return SKY_ERROR_DETOURS_INIT;
    }

    status = Detours::CreateHook(m_d3d9DeviceVTable[42], &OnEndScene, reinterpret_cast<void**>(&s_oEndSceneFunction));
    if (status != MH_OK)
    {
#if _DEBUG
        m_logger->LogDebug("Failed to initialze create OnEndScene hook: %s", Detours::StatusToString(status));
#endif
        return SKY_ERROR_DETOURS_HOOK_ENDSCENE;
    }

    status = Detours::CreateHook(m_d3d9DeviceVTable[16], &OnReset, reinterpret_cast<void**>(&s_oResetFunction));
    if (status != MH_OK)
    {
#if _DEBUG
        m_logger->LogDebug("Failed to initialze create OnReset hook: %s", Detours::StatusToString(status));
#endif
        return SKY_ERROR_DETOURS_HOOK_RESET;
    }

    status = Detours::EnableAllHooks();
    if (status != MH_OK)
    {
#if _DEBUG
        m_logger->LogDebug("Failed to enable all hooks: %s", Detours::StatusToString(status));
#endif
        return SKY_ERROR_DETOURS_ENABLE;
    }

    // ==============
    //if (MH_CreateHook(&MessageBoxW, &DetourMessageBoxW,
    //    reinterpret_cast<LPVOID*>(&fpMessageBoxW)) != MH_OK)
    //{
    //    return 1;
    //}
    //if (MH_EnableHook(&MessageBoxW) != MH_OK)
    //{
    //    return 1;
    //}
    //MessageBoxW(NULL, L"Not hooked...", L"MinHook Sample", MB_OK);
    // ==============

    return SKY_SUCCESS;

    //memcpy(m_originalEndSceneBytes, (char*)m_d3d9Device[42], 7);
    //s_oEndSceneFunction = (EndSceneFunction)Detours::TrampHook((char*)m_d3d9Device[42], (char*)&OnEndScene, 7);
    //if (!s_oEndSceneFunction)
    //    return SKY_ERROR_ENDSCENE_HOOK;
}

void DirectXHelper::UnhookMethods()
{
//#ifdef _DEBUG
//    if (!Detours::DisableAllHooks())
//        Debug::LogErr("Detours::DisableAllHooks failed");
//
//    if (!Detours::Destroy())
//        Debug::LogErr("Detours::Destroy failed");
//#else
//    Detours::DisableAllHooks();
//    Detours::Destroy();
//#endif

    MH_STATUS status = Detours::DisableAllHooks();
    if (status != MH_OK)
    {
#ifdef _DEBUG
        m_logger->LogCritical("Failed to disable all hooks: %s", Detours::StatusToString(status));
#endif
    }

    status = Detours::Destroy();
    if (status != MH_OK)
    {
#ifdef _DEBUG
        m_logger->LogCritical("Failed to uninitialize detours: %s", Detours::StatusToString(status));
#endif
    }

}
