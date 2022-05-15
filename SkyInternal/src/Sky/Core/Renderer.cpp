#include "Renderer.h"

#include "Utils.h"

using namespace Sky::Core;

// Static variables
WndProcFunction Renderer::s_oWndProcFunction = nullptr;

Renderer::Renderer(Engine& engine)
    : m_engine(engine), m_showMenu(false),
    m_window(), m_d3dWindow(),
    m_windowWidth(0), m_windowHeight(0),
    m_directXhelper(), m_imGuiHelper(), m_visuals()
{
}

Renderer::~Renderer()
{
    // Reset window hook
#ifdef _DEBUG
    SetLastError(0);
    long hookedWndProc = SetWindowLongW(m_window, GWL_WNDPROC, (LONG_PTR)s_oWndProcFunction);
    if (!hookedWndProc)
    {
        m_logger->LogCritical(Logger::GetLastError, "SetWindowLongW failed");
    }
#else
    SetWindowLongW(m_window, GWL_WNDPROC, (LONG_PTR)s_oWndProcFunction);
#endif

    // Unhook
    if (m_directXhelper)
        delete m_directXhelper;

    // Cleanup ImGui
    if (m_imGuiHelper)
        delete m_imGuiHelper;

    if (m_visuals)
        delete m_visuals;
}

HRESULT Renderer::Create()
{
    HRESULT hr;

    // Setup window
    hr = InitWindow();
    if (hr != SKY_SUCCESS)
        return hr;

    m_directXhelper = new DirectXHelper(m_window);
    hr = m_directXhelper->Create();
    if (hr != SKY_SUCCESS)
        return hr;

    SKY_DBG(SetLastError(0));
    s_oWndProcFunction = (WndProcFunction)SetWindowLongPtr(m_window, GWL_WNDPROC, (LONG_PTR)WndProc);
    if (!s_oWndProcFunction)
    {
        SKY_DBG(m_logger->LogDebug(Logger::GetLastError, "SetWindowLongPtr failed"));
        return SKY_ERROR_WINDOW_PROC;
    }

    m_visuals = new Visuals(m_windowWidth, m_windowHeight);

    return SKY_SUCCESS;
}

HRESULT Renderer::InitWindow()
{
    // Get window
    m_window = NULL;
    EnumWindows(EnumWindowsCallback, reinterpret_cast<LPARAM>(this));
    if (m_window == NULL)
        return SKY_ERROR_WINDOW;

    // Get size
    SKY_DBG(SetLastError(0));
    RECT size;
    if (!GetClientRect(m_window, &size))
    {
        SKY_DBG(m_logger->LogDebug(Logger::GetLastError, "GetClientRect failed"));
        return SKY_ERROR_WINDOW_SIZE;
    }

    m_windowWidth = size.right - size.left;
    m_windowHeight = size.bottom - size.top;

    return SKY_SUCCESS;
}

void Renderer::OnRender(IDirect3DDevice9* device)
{
    m_visuals->m_device = device;

    // Init ImGui
    if (!m_imGuiHelper)
    {
        //D3DDEVICE_CREATION_PARAMETERS pp;
        //HRESULT hr = device->GetCreationParameters(&pp);
        //if (!FAILED(hr))
        //{
        //    auto ss = std::stringstream();
        //    ss << "hWnd: " << pp.hFocusWindow;
        //    Debug::MsgBox(ss.str());

        //    s_oWndProcFunction = (WndProcFunction)SetWindowLongPtr(pp.hFocusWindow, GWL_WNDPROC, (LONG_PTR)WndProc);
        //    if (!s_oWndProcFunction)
        //    {
        //        m_engine->Exit();
        //        //return SKY_ERROR;
        //    }
        //}
        //else
        //{
        //    auto ss = std::stringstream();
        //    ss << "HR: " << hr << " | D3DERR_INVALIDCALL: " << D3DERR_INVALIDCALL;
        //    Debug::MsgBox(ss.str());

        //    m_engine->Exit();
        //}

        m_imGuiHelper = new ImGuiHelper(m_window, device);
    }

    if (m_showMenu)
    {
        // Start new ImGui frame
        m_imGuiHelper->NewFrame();

        // Draw menu
        ImGui::Begin("Menu", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        if (ImGui::BeginTabBar("Modules", ImGuiTabBarFlags_None))
        {
            // Update modules for menu
            m_engine.OnMenuRender();

            ImGui::EndTabBar();
        }
        ImGui::End();

        // End ImGui frame
        m_imGuiHelper->EndFrame();
    }

    // Update modules
    m_engine.OnRender(*m_visuals);

}

void Renderer::OnDeviceLost()
{
    if (m_imGuiHelper)
        m_imGuiHelper->OnDeviceLost();
}

void Renderer::OnDeviceReset()
{
    if (m_imGuiHelper)
        m_imGuiHelper->OnDeviceReset();
}

// Static methods
LRESULT __stdcall Renderer::WndProc(const HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Engine& engine = Engine::Instance();

    // Engine main loop
    engine.OnWindowUpdate(hWnd, msg, wParam, lParam);
    
    // Handle ImGui
    if (ImGuiHelper::WndProc(hWnd, msg, wParam, lParam))
    {
        Renderer& renderer = engine.GetRenderer();
        // Only skip default routine if menu is open
        if (renderer.m_showMenu)
            return true;
    }

    return s_oWndProcFunction(hWnd, msg, wParam, lParam);   // not detectable and way more secure, seems to be working
    //return CallWindowProcW(s_oWndProcFunction, hWnd, msg, wParam, lParam);
}


BOOL Renderer::EnumWindowsCallback(HWND handle, LPARAM lParam)
{
    DWORD wndProcId;
    GetWindowThreadProcessId(handle, &wndProcId);

    if (GetCurrentProcessId() != wndProcId || !IsWindowVisible(handle))
        return TRUE;    // skip to next window

    // Get window title and make sure it doesn't contains ".exe"
    // Basically imitating the behavior of:
    // FindWindowA(NULL, nameOfTheWindow);
    std::string title(GetWindowTextLengthA(handle) + 1, L'\0');
    GetWindowTextA(handle, &title[0], title.size());

    if (Utils::StringContains(title, ".exe"))
    {
        return TRUE;    // skip to next window
    }

    reinterpret_cast<Renderer*>(lParam)->m_window = handle;

    return FALSE; // window found abort search
}