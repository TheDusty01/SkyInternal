#include "GameData.h"

#include "../Core/Memory.h"
#include "../../ExitCodes.h"

using namespace Sky::Core;
using namespace Sky::Game;

GameData::GameData()
	: m_entities()
{
	// Init entity list
	for (size_t i = 0; i < 64; i++)
	{
		m_entities[i] = new BaseEntity(Memory::s_clientAddress + Offsets::Signatures::dwEntityList + (i * BaseEntity::GetObjectSize()));
	}
	memcpy(&entitesWithoutPlayer, &m_entities[1], sizeof(entitesWithoutPlayer));
}

GameData::~GameData()
{
	for (auto& entity : m_entities)
	{
		delete entity;
		// No need to iterate over both arrays since the other array is just a shallow copy of the current one.
		// Deleting the entity here, clears up resources for both arrays.
	}
}

std::array<BaseEntity*, 64> GameData::GetEntityList() const
{
	return m_entities;
}

std::array<BaseEntity*, 63> GameData::GetEntityListWithoutPlayer() const
{
	return entitesWithoutPlayer;
}

const BaseEntity* GameData::GetEntity(uint8_t index)
{
	if (index >= 0 && index < 64)
		return m_entities[index];

	return nullptr;
}

const BaseEntity* GameData::GetLocalPlayer()
{
	return GetEntity(0);
}

ViewMatrix GameData::GetViewMatrix() const
{
	return Memory::Read<ViewMatrix>(Memory::s_clientAddress + Offsets::Signatures::dwViewMatrix);
}
