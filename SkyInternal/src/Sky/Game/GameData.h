#pragma once

#include "../Core/Math.h"

#include "Entity.h"

#include <Windows.h>
#include <array>

namespace Sky
{
	using namespace Core;

	namespace Game
	{

		class GameData
		{
		// Fields
		private:
#pragma region API data
			std::array<BaseEntity*, 64> m_entities;
			std::array<BaseEntity*, 63> entitesWithoutPlayer;
#pragma endregion


		// Methods
		public:
			GameData();
			GameData(const GameData&) = delete;
			~GameData();

#pragma region API
			std::array<BaseEntity*, 64> GetEntityList() const;
			std::array<BaseEntity*, 63> GetEntityListWithoutPlayer() const;
			const BaseEntity* GetEntity(uint8_t index);
			const BaseEntity* GetLocalPlayer();

			ViewMatrix GetViewMatrix() const;
#pragma endregion
		};

	}
}

