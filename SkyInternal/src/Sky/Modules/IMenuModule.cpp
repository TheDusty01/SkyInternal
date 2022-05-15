#include "IMenuModule.h"

using namespace Sky::Modules;

//void Sky::Modules::IMenuModule::OnInitialize()
//{
//}
//
//void Sky::Modules::IMenuModule::OnUpdate()
//{
//}
//
//void Sky::Modules::IMenuModule::OnRender()
//{
//}

void IMenuModule::OnMenuRender()
{
	if (ImGui::BeginTabItem(m_name.c_str()))
	{
		ImGui::Checkbox("Enabled", &m_isEnabled._Storage._Value);

		OnMenuContentRender();
		
		ImGui::EndTabItem();
	}
}
