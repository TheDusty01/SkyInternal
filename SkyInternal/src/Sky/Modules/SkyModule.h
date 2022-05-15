#pragma once

#include "../Core/Engine.h"
#include "../Core/Input.h"
#include "../Core/Visuals.h"
#include "../Game/GameData.h"

#include <atomic>
#include <string_view>

namespace Sky
{
	// Forward declaration
	namespace Core
	{
		class Engine;
	}
	using namespace Sky::Core;

	using namespace Sky::Game;

	namespace Modules
	{

		class SkyModule
		{
			friend class Engine;

		// Fields
		private:
			const Engine* m_engine;
		protected:
			GameData* m_gameData;
			std::atomic_bool m_isEnabled;
			std::string m_name = "Unknown";

		public:

		// Methods
		private:
			void Initialize(Engine& engine, GameData& gameData);

		public:
			SkyModule();
			virtual ~SkyModule();

			// Events
			virtual void OnInitialize() = 0;
			virtual void OnEnable();
			virtual void OnDisable();

			virtual void OnUpdate() = 0;	// WndProc loop
			virtual void OnMenuRender() = 0;
			virtual void OnRender(const Visuals& visuals) = 0;


			void Toggle();
			
			// Getter & Setter
			bool IsEnabled();

		};

	}

}