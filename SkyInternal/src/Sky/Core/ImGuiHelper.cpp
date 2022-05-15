#include "ImGuiHelper.h"

using namespace Sky::Core;

ImGuiHelper::ImGuiHelper(HWND window, IDirect3DDevice9* device)
{
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();

    // Init backends
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);

    m_isReady = true;
}

ImGuiHelper::~ImGuiHelper()
{
    // Shutdown backends
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}

void ImGuiHelper::OnDeviceLost()
{
    m_isReady = false;
    ImGui_ImplDX9_InvalidateDeviceObjects();
}

void ImGuiHelper::OnDeviceReset()
{
    ImGui_ImplDX9_CreateDeviceObjects();
    m_isReady = true;
}

void ImGuiHelper::NewFrame()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiHelper::EndFrame()
{
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT ImGuiHelper::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}
