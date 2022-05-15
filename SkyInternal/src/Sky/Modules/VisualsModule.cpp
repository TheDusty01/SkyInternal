#include "VisualsModule.h"

#include "../Core/Memory.h"

#include "../Core/Debug.h"

#include "../Game/Entity.h"
#include "../Core/Math.h"

using namespace Sky::Core;
using namespace Sky::Modules;
using namespace Sky::Game;
using namespace Sky::Game::Offsets;

void VisualsModule::OnInitialize()
{
    m_name = "Visuals";
	m_pageCount = 3;
	
	//m_pages.insert(std::pair<uint8_t, PageFunction>(0, &VisualsModule::OnMenuPageRender));
}

void VisualsModule::OnUpdate()
{

}

//void DemoModule2::OnRender(const Visuals& visuals)
//{
//	//ViewMatrix viewMatrix = Engine::ReadMemory<ViewMatrix>(Engine::s_clientAddress + Offsets::Signatures::dwViewMatrix);
//	Matrix4x4 vm = Engine::ReadMemory<Matrix4x4>(Engine::s_clientAddress + Offsets::Signatures::dwViewMatrix);
//
//	uintptr_t entSize = 0x10;
//	for (int i = 1; i < 2; i++)
//	{
//		//uintptr_t ent = Engine::ReadMemory<uintptr_t>(Engine::s_clientAddress + Offsets::Signatures::dwEntityList + (i * entSize));
//		uintptr_t ent = *(uintptr_t*)(Engine::s_clientAddress + Offsets::Signatures::dwEntityList + (i * entSize));
//		if (!ent)
//			continue;
//
//
//		Vec3f feetPos = *(Vec3f*)(ent + Offsets::Netvars::m_vecOrigin);
//		//m_logger->LogInfo("FEET - X: %d, Y: %d, Z: %d", feetPos(0), feetPos(1), feetPos(2));
//		Vec3f headPos = { feetPos.x, feetPos.y, feetPos.z + 75.0f };
//
//		Vec2f screenBottom;
//		if (!visuals.WorldToScreen4(feetPos, screenBottom, vm.data, 1920, 1080))
//		{
//			m_logger->LogDebug("WorldToScreen feetPos failed");
//			continue;
//		}
//
//		Vec2f screenTop;
//		if (!visuals.WorldToScreen4(headPos, screenTop, vm.data, 1920, 1080))
//		{
//			m_logger->LogDebug("WorldToScreen headPos failed");
//			continue;
//		}
//
//
//		float height = screenBottom.y - screenTop.y;
//		//float height = screenTop.y - screenBottom.y;
//		float width = height / 2.4f;
//
//		bool isDormant = *(bool*)(ent + Offsets::Signatures::m_bDormant);
//		if (!isDormant)
//		{
//			// WORKS
//			visuals.DrawBox(screenTop.x - (width / 2), screenTop.y, width, height, 1, Color::FromRGB(255, 0, 0));
//		}
//		else
//		{
//			//m_logger->LogInfo("Entity is dormant: Index %d", i);
//		}
//	}
//}

//void VisualsModule::OnRender(const Visuals& visuals)
//{
//	ViewMatrix viewMatrix = Memory::Read<ViewMatrix>(Memory::s_clientAddress + Offsets::Signatures::dwViewMatrix);
//
//	uintptr_t entSize = 0x10;
//	for (int i = 1; i < 64; i++)
//	{
//		BaseEntity ent = BaseEntity(Memory::s_clientAddress + Offsets::Signatures::dwEntityList + (i * entSize));
//		if (!ent.IsAddressValid())
//			continue;
//
//		if (!ent.IsValidEntity())
//			continue;
//
//		Vector3f feetPos = ent.GetFeetPosition();
//		Vector3f headPos = { feetPos[0], feetPos[1], feetPos[2] + 75.0f };
//		//Vector3f headPos = ent.GetHeadPosition();
//
//		Vector2f screenBottom;
//		if (!visuals.WorldToScreen(feetPos, screenBottom, viewMatrix, 1920, 1080))
//		{
//#if _DEBUG
//			m_logger->LogDebug("WorldToScreen feetPos failed");
//#endif
//			continue;
//		}
//
//		Vector2f screenTop;
//		if (!visuals.WorldToScreen(headPos, screenTop, viewMatrix, 1920, 1080))
//		{
//#if _DEBUG
//			m_logger->LogDebug("WorldToScreen headPos failed");
//#endif
//			continue;
//		}
//
//		// Draw box
//		float height = screenBottom[1] - screenTop[1];
//		float width = height / 2.4f;
//		visuals.DrawBox(screenTop[0] - (width / 2), screenTop[1], width, height, 1, Color::FromRGB(255, 0, 0));
//	}
//}

void VisualsModule::OnRender(const Visuals& visuals)
{
	ViewMatrix viewMatrix = m_gameData->GetViewMatrix();
	auto entities = m_gameData->GetEntityListWithoutPlayer();
	for (const auto& ent : entities)
	{
		if (!ent->IsAddressValid())
			continue;

		if (!ent->IsValidEntity())
			continue;

		Vector3f feetPos = ent->GetFeetPosition();
		Vector3f headPos = ent->GetHeadPosition();

		Vector2f screenBottom;
		if (!visuals.WorldToScreen(feetPos, screenBottom, viewMatrix))
		{
			SKY_DBG(m_logger->LogDebug("WorldToScreen feetPos failed"));
			continue;
		}

		Vector2f screenTop;
		if (!visuals.WorldToScreen(headPos, screenTop, viewMatrix))
		{
			SKY_DBG(m_logger->LogDebug("WorldToScreen headPos failed"));
			continue;
		}

		// Draw box
		float height = screenBottom[1] - screenTop[1];
		float width = height / 2.4f;
		visuals.DrawBox(screenTop[0] - (width / 2), screenTop[1], width, height, 1, Color::FromRGB(255, 0, 0));
	}
}


//void VisualsModule::OnMenuContentRender()
//{
//	ImGui::Text("Visuals tab!\n2nd line");
//}

void VisualsModule::OnMenuPageRender(uint8_t pageIndex)
{
	switch (pageIndex)
	{
	case 0:
		ImGui::Text("Visuals tab!\nPage 1");
		break;
	case 1:
		ImGui::Text("Visuals tab!\nPage 2");
		break;
	case 2:
		ImGui::Text("Visuals tab!\nPage 3");
	}
}

//
//void VisualsModule::OnMenuMainPageRender()
//{
//	ImGui::Text("Visuals tab!\MAIN PAGE");
//}
