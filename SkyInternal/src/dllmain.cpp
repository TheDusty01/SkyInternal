#include <Windows.h>

#include "Sky/Core/Engine.h"

#ifdef _DEBUG
#include "Sky/Core/Debug.h"
#include "Sky/Modules/DemoModule.h"
#include "Sky/Modules/VisualsModule.h"
#endif

using namespace Sky;
using namespace Sky::Modules;

void SetupModules(Engine& engine)
{
#ifdef _DEBUG
    engine.AddModule<DemoModule>();
    engine.AddModule<VisualsModule>();
#endif
}

DWORD WINAPI Init(HMODULE hModule)
{
#ifdef _DEBUG
    Debug::CreateConsole();
    Logger::SetDefaultLogLevel(LogLevel::Information);
    const std::shared_ptr<Logger> const logger = Logger::Create("Main");
#endif
    Engine& engine = Engine::Instance();
    
    // Setup engine
    HRESULT hr = engine.Create();
    if (hr == SKY_SUCCESS)
    {
        SetupModules(engine);

        // Main loop
        engine.Run();
        
    }
    #ifdef _DEBUG
    else
    {
        logger->LogCritical(hr, "Init failed");
    }
    #endif

    // Free memory
    Engine::Destroy();

#ifdef _DEBUG
    //Debug::MsgBox("Wait");
    Debug::DestroyConsole();
#endif


    // Uninject
    FreeLibraryAndExitThread(hModule, 0);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Do not get notified by thread attach-/detachment calls
        DisableThreadLibraryCalls(hModule);

        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Init, hModule, 0, nullptr));
    
    case DLL_THREAD_ATTACH: // Ignored
    case DLL_THREAD_DETACH: // Ignored
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

