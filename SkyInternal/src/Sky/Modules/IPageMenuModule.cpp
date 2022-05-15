#include "IPageMenuModule.h"


using namespace Sky::Core;
using namespace Sky::Modules;
using namespace Sky::Game;
using namespace Sky::Game::Offsets;

//void IPageMenuModule::AddOrReplacePage(uint8_t pageIndex, PageFunction callback)
//{
//	m_pages[pageIndex] = callback;
//}
//
//void IPageMenuModule::AddPage(PageFunction callback)
//{
//	AddOrReplacePage(m_pages.size(), callback);
//}

void IPageMenuModule::OnMenuContentRender()
{
	OnMenuPageRender(m_activePage);

	for (uint8_t i = 0; i < m_pageCount; i++)
	{
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%s %d", "Page ", i + 1);
		if (ImGui::Button(buffer))
		{
			// Set new active page
			m_activePage = i;
		}
	}
}

//void IPageMenuModule::OnMenuPageRender(uint8_t pageIndex)
//{
//	m_pages[pageIndex](pageIndex);
//}
