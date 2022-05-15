#include "DemoModule.h"

#include "../Core/Memory.h"

#include "../Core/Debug.h"

using namespace Sky::Core;
using namespace Sky::Modules;
using namespace Sky::Game;
using namespace Sky::Game::Offsets;

//const std::string_view DemoModule::s_name = "Wallhack";
//const char* DemoModule::s_name = "Wallhack";

void DemoModule::OnInitialize()
{
    m_name = "Wallhack";
    this->Toggle();

    m_logger->LogInfo(TYPE_NAME(OnInitialize));
}


void DemoModule::OnUpdate()
{
   //m_logger->LogInfo("OnUpdate");
}

void DemoModule::OnRender(const Visuals& visuals)
{
    //m_logger->LogInfo("OnRender");

    bool isKeyDown = Input::IsKeyDownOnce(VK_F2);
    //bool isKeyDown = Input::IsKeyDown(VK_F2);
    if (isKeyDown)
    {
        uintptr_t lp = *(uintptr_t*)(Memory::s_clientAddress + Offsets::Signatures::dwLocalPlayer);

        uintptr_t localPlayer = Memory::Read<uintptr_t>(Memory::s_clientAddress + Offsets::Signatures::dwLocalPlayer);

        if (localPlayer)
        {
            int hp = Memory::Read<int>(localPlayer + Offsets::Netvars::m_iHealth);

            m_logger->LogInfo("DemoModule::OnUpdate - HP: %d", hp);
        }
        else
        {
            m_logger->LogInfo("DemoModule::OnUpdate - localPlayer null");
        }
    }

    //if (Input::IsKeyDown(VK_F2))
    //{
    //    Debug::Log("DemoModule::OnUpdate - F2 down");
    //}

    //uintptr_t localPlayer = Engine::ReadMemory<uintptr_t>(Engine::s_clientAddress + Offsets::Signatures::dwLocalPlayer);

    //if (localPlayer)
    //{
    //    int hp = Engine::ReadMemory<int>(localPlayer + Offsets::Netvars::m_iHealth);

    //    auto ss = std::stringstream();
    //    ss << "DemoModule::OnUpdate - " << "HP: " << hp;
    //    Debug::Log(ss.str());
    //}
    //else
    //{
    //    Debug::Log("DemoModule::OnUpdate - localPlayer null");
    //}

}

void DemoModule::OnMenuRender()
{
	//ImGui::ShowDemoWindow();

    if (ImGui::BeginTabItem(m_name.c_str()))
    {
        ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");

        ImGui::Checkbox("Enabled", &m_isEnabled._Storage._Value);

        //if (ImGui::BeginTabBar("DemoModuleSettings"))
        //{
        //    if (ImGui::BeginTabItem("Broccoli"))
        //    {
        //        ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
        //        ImGui::EndTabItem();
        //    }
        //    if (ImGui::BeginTabItem("Cucumber"))
        //    {
        //        ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
        //        ImGui::EndTabItem();
        //    }

        //    ImGui::EndTabBar();
        //}

        ImGui::EndTabItem();
    }

    // Testing
    //if (ImGui::BeginTabItem("Broccoli"))
    //{
    //    ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
    //    ImGui::EndTabItem();
    //}
    //if (ImGui::BeginTabItem("Cucumber"))
    //{
    //    ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
    //    ImGui::EndTabItem();
    //}

}
