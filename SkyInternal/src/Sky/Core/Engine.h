#pragma once

#include "../../ExitCodes.h"
#include "Renderer.h"
#include "Visuals.h"
#include "../Modules/SkyModule.h"
#include "Input.h"

#include "../Game/GameData.h"

#if _DEBUG
#include "Debug.h"
#endif

#include <Windows.h>
#include <vector>


namespace Sky
{
	// Forward declaration
	namespace Modules
	{
		class SkyModule;
	}
	using namespace Sky::Modules;

	using namespace Sky::Game;

	namespace Core
	{
		// Forward declaration
		class Renderer;

		class Engine
		{
			friend class Renderer;


		// Fields
		private:
#if _DEBUG
			const std::shared_ptr<Logger> const m_logger = Logger::Create(TYPE_NAME(Engine));
#endif
			static Engine* s_instance;

			Renderer* m_renderer;
			GameData* m_gameData;
			std::vector<SkyModule*> m_modules;

			std::atomic_bool m_isRunning;
		
		// Methods
		private:
			Engine();
			Engine(const Engine&) = delete;
			Engine& operator=(const Engine&) = delete;
			~Engine();

			void __stdcall OnWindowUpdate(const HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

			void OnMenuRender();
			void OnRender(const Visuals& visuals) const;
		public:
			static Engine& Instance();
			static void Destroy();

			HRESULT Create();
			void Run();
			void RequestExit();

			Renderer& GetRenderer();

#pragma region Modules
			template <class T>
			void AddModule()
			{
				static_assert(std::is_base_of<SkyModule, T>::value, "T should inherit from SkyModule");

				T* module = new T();
				SkyModule* moduleBase = (SkyModule*)module;
				moduleBase->Initialize(*this, *m_gameData);
				m_modules.push_back(moduleBase);
			}

			template <class T>
			void RemoveModule()
			{
				static_assert(std::is_base_of<SkyModule, T>::value, "T should inherit from SkyModule");

				for (std::vector<SkyModule*>::iterator it = m_modules.begin(); it != m_modules.end(); )
				{
					if (dynamic_cast<T*>(*it))
					{
						delete* it;
						m_modules.erase(it);
						return;
					}
					else
					{
						++it;
					}
				}
			}
#pragma endregion

		};

	}
}
