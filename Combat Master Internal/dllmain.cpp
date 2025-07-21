#include "pch/pch.h"

void InitCheat()
{
    AllocConsole();
    freopen("conout$", "w", stdout);

    printf("[Project-V5.0] Injected!\n");

    Globals::Init();

    PatchAntiDebug();

    GUI::HookPresent();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        InitCheat();
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        // Optional -> Cleanup all made memory allocations
        // Remove hooks, etc..
    }
    return TRUE;
}
