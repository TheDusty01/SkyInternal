#include "Entity.h"

#pragma once
#pragma warning (disable : 4005)

#define FLAG_DUCKING (1<<1) // Is crouching?


using namespace Sky::Core;
using namespace Sky::Game;
using namespace Sky::Game::Offsets;

#pragma region General
uint8_t BaseEntity::GetHealth() const
{
	return ReadMemory<uint8_t>(Netvars::m_iHealth);
}

uint8_t BaseEntity::GetTeam() const
{
	// Another example
	//return Driver.ReadVirtualMemory<uint8_t>(ProcessId, EntityAddress + m_iTeamNum, sizeof(uint8_t));
	return ReadMemory<uint8_t>(Netvars::m_iTeamNum);
}
#pragma endregion

#pragma region Position
Vector3f BaseEntity::GetViewOffset() const
{
	return ReadMemory<Vector3f>(Netvars::m_vecViewOffset);
}

Vector3f BaseEntity::GetHeadPosition() const
{
	Vector3f feetPos = GetFeetPosition();
	//Vector3f viewOffset = GetViewOffset();

	//Vector3f localEyeOrigin = feetPos + viewOffset;
	if (IsCrouching())
	{
		//localEyeOrigin(1) = localEyeOrigin(1) - 1.5f;
		//headPos[2] =- 15.0f;
		return Vector3f { feetPos[0], feetPos[1], feetPos[2] + 55.0f };
	}
	else
	{
		return Vector3f { feetPos[0], feetPos[1], feetPos[2] + 75.0f };
	}

	//return localEyeOrigin;
}

Vector3f BaseEntity::GetFeetPosition() const
{
	return ReadMemory<Vector3f>(Netvars::m_vecOrigin);
}
#pragma endregion

#pragma region Flags

uint32_t BaseEntity::GetFlags() const
{
	return ReadMemory<uint32_t>(Netvars::m_fFlags);
}

bool BaseEntity::IsValidEntity() const
{
	uint8_t health = GetHealth();
	bool isDormant = IsDormant();
	return health > 0 && health <= 100 && !isDormant;
}

bool BaseEntity::IsDormant() const
{
	return ReadMemory<bool>(Signatures::m_bDormant);
}

bool BaseEntity::IsCrouching() const
{
	uint32_t flags = GetFlags();
	return flags & FL_DUCKING;
}
#pragma endregion
