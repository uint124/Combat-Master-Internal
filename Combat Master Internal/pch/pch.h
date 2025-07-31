#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <winternl.h>
#include <unordered_map>
#include <d3d11.h>
#include <D3DX11.h>

#ifndef _IMGUI
#define _IMGUI

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"

#endif

#ifndef _DETOURS
#define _DETOURS

#include "detours.h"

#endif

#include "Game/Types/Vector.h"
#include "Game/Types/Enums.h"
#include "Game/Types/Structs.h"

#include "Game/Offsets.h"
#include "Game/Globals.h"

#include "Utils/Signature.h"
#include "Utils/CalcAngle.h"
#include "Utils/Anticheat.h"

#include "Game/SDK/List.h"
#include "Game/SDK/Il2cpp.h"
#include "Game/SDK/Object.h"
#include "Game/SDK/GameObject.h"
#include "Game/SDK/Transform.h"
#include "Game/SDK/Camera.h"
#include "Game/SDK/CameraController.h"
#include "Game/SDK/PlayerMovement.h"
#include "Game/SDK/PlayerHealth.h"
#include "Game/SDK/PlayerRoot.h"

#include "Utils/CombatMaster.h"
#include "Utils/DebugLog.h"
#include "Utils/InstanceFinder.h"

#include "Render/GUI/Initialize.h"
#include "Render/GUI/Style.h"
#include "Render/GUI/Present.h"
