#include "pch/pch.h"

void InitCheat()
{
    AllocConsole();
    freopen("conout$", "w", stdout);

    printf("[Project-V5.0] Injected!\n");

    Globals::Init();
    
    auto func = getFunc("UnityEngine.Physics::IgnoreCollision_Injected(System.IntPtr,System.IntPtr,System.Boolean)");
    auto func2 = getFunc("UnityEngine.Transform::Internal_LookAt_Injected(System.IntPtr,UnityEngine.Vector3&,UnityEngine.Vector3&)");
    printf("Function: %llx | gravity func: %llx\n", func, func2);


    // Physics Manager 1F09258
    PatchAntiDebug();

    //FindPlayerRootReferences();

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
