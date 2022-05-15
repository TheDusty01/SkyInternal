#include "Engine.h"

#include "Memory.h"

using namespace Sky::Core;
using namespace Sky::Game;

// Static fields
Engine* Engine::s_instance = nullptr;

Engine& Engine::Instance()
{
	if (s_instance)
		return *s_instance;

	s_instance = new Engine();
}

void Engine::Destroy()
{
	if (s_instance)
	{
		delete s_instance;
		s_instance = nullptr;
	}
}

Engine::Engine()
	: m_isRunning(false), m_gameData()
{
	m_renderer = new Renderer(*this);
}

Engine::~Engine()
{
	// Make sure OnUpdate and OnMenuRender aren't getting called
	delete m_renderer;

	for (auto& module : m_modules)
	{
		delete module;
		// No need to remove from list since vector is cleared up after the destructor anyways
	}

	delete m_gameData;
}

void __stdcall Engine::OnWindowUpdate(const HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Only accept calls if engine is running
	if (!m_isRunning)
		return;

	// Handle input
	Input::InputMessage inputMsg = Input::GetInputMessage(msg, wParam, lParam);
	if (!inputMsg.IsValid())
		return;	// Return if update is not a input message

	// Uninject
	if (Input::IsKeyDown(VK_END))
	{
		// Eject
		RequestExit();	// non blocking
		return;
	}

	// Menu toggle
	if (Input::IsKeyDown(VK_F8))
	{
#if _DEBUG
		m_logger->LogInfo("INPUT DOWN VK_F8");
#endif

		m_renderer->m_showMenu = !m_renderer->m_showMenu;
	}

	// TODO: Use ImGui Input in future, way more reliable and faster for some reason..
	//	if (ImGui::IsKeyDown(VK_F8))
	//	{
	//#if _DEBUG
	//		m_logger->LogInfo("IMGUI DOWN VK_F8");
	//#endif
	//		m_renderer->m_showMenu = !m_renderer->m_showMenu;
	//	}

	// Update modules
	for (auto& module : m_modules)
	{
		// Only update active modules
		if (module->IsEnabled())
			module->OnUpdate();
	}

	// Process input for the next update
	//Input::ProcessInput(msg, wParam, lParam);
	Input::ProcessInput(inputMsg);
}

void Engine::OnMenuRender()
{
	// Only accept calls if engine is running
	if (!m_isRunning)
		return;

	// Update modules
	for (auto& module : m_modules)
	{
		module->OnMenuRender();
	}

}

void Engine::OnRender(const Visuals& visuals) const
{
	// Only accept calls if engine is running
	if (!m_isRunning)
		return;

	// Update modules
	for (auto& module : m_modules)
	{
		// Only update active modules
		if (module->IsEnabled())
			module->OnRender(visuals);
	}

	// Maybe do some other stuff internally
}

HRESULT Engine::Create()
{
	// Init memory
	HRESULT hr;
	hr = Memory::LoadModules();
	if (hr != SKY_SUCCESS)
		return hr;

	// Init renderer
	hr = m_renderer->Create();
	if (hr != SKY_SUCCESS)
	{
		return hr;
	}

	// Init GameData
	m_gameData = new GameData();

	return hr;
}

void Engine::Run()
{
	m_isRunning = true;

	// TODO: Should be a mutext in the future instead of a while loop with sleep

	while (m_isRunning/* && !GetAsyncKeyState(VK_END)*/)
	{
		//Sleep(10);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void Engine::RequestExit()
{
	m_isRunning = false;
}

Renderer& Engine::GetRenderer()
{
	return *m_renderer;
}