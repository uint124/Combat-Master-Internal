#pragma once

//char __fastcall sub_36DF970(int* a1, __int64 a2, unsigned __int64* a3, __int64 a4, unsigned int a5, int a6);
//bool __fastcall Internal_RaycastTest_Injected_10FB60(int* a1, __int64 a2, float a3, int a4, int a5)
uint64_t* getFunc(const char* name)
{
    using Fn = uint64_t * (__fastcall*)(const char*);
    auto func = reinterpret_cast<Fn>(ProjectModule + 0x4CD510);
    return func(name);
}

__int64 GetDefaultPhysicsScene()
{
    using Fn = __int64(__fastcall*)();
    auto getDefaultPhysicsScene_fn = reinterpret_cast<Fn>(ProjectModule + 0x36D88D0);

    auto retval = getDefaultPhysicsScene_fn();

    return retval;
}

class Ray
{
public:
    Vector3 origin;
    Vector3 direction;
};

typedef bool(__fastcall* RaycastFuncType)(int*, Ray*, float, int, int);
RaycastFuncType originalRaycast;
bool RaycastFunc(int* physicsScene, Ray* ray, float maxDistance, int layerMask, int queryTriggerInteraction)
{
    auto localPlayer = PlayerRoot::GetLocalPlayer();
    auto camera = localPlayer->GetCamera();

    auto retval = originalRaycast(physicsScene, ray, maxDistance, layerMask, queryTriggerInteraction);
    printf("[Raycast]\nPhysics Scene: %llx\nRay Origin: [%.0f, %.0f, %.0f]\nRay Direction: [%.0f, %.0f, %.0f]\nLayerMask: %i\nQuery: %i\nDistance: %.0f\nReturnValue: %s\n", physicsScene, ray->origin.x, ray->origin.y, ray->origin.z, ray->direction.x, ray->direction.y, ray->direction.z, layerMask, queryTriggerInteraction, maxDistance, retval ? "TRUE" : "FALSE");

    maxDistance = 10.f;
    auto newValue = originalRaycast(physicsScene, ray, maxDistance, layerMask, queryTriggerInteraction);

    return retval;
}

namespace GUI
{
    bool init = false;
    HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
    {
        if (!init)
        {
            if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
            {
                pDevice->GetImmediateContext(&pContext);
                DXGI_SWAP_CHAIN_DESC sd;
                pSwapChain->GetDesc(&sd);
                window = sd.OutputWindow;
                ID3D11Texture2D* pBackBuffer;
                pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
                pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
                pBackBuffer->Release();
                oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
                ImGui::CreateContext();
                ImGui_ImplWin32_Init(window);
                ImGui_ImplDX11_Init(pDevice, pContext);
                InitMenuStyle(pDevice);

                init = true;
            }

            else {
                return oPresent(pSwapChain, SyncInterval, Flags);
            }
        }

        // Rendering Menu
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImDrawList* BackgroundDrawList = ImGui::GetBackgroundDrawList();
        ImDrawList* ForegroundDrawList = ImGui::GetForegroundDrawList();

        if (Menu::bDrawFov) {
            BackgroundDrawList->AddCircle(ImVec2(Screen::ScreenCenter.x, Screen::ScreenCenter.y), Menu::aimbotFov, IM_COL32(255, 255, 255, 255), 100, 1.5f);
        }

        if (GetAsyncKeyState(VK_INSERT) & 1) {
            Menu::bIsOpen = !Menu::bIsOpen;
        }

        if (Menu::bWatermark) {
            char buffer[42];
            snprintf(buffer, sizeof(buffer), "Build Time: %s | Debugger: %s\n", __TIME__, IsDebuggerPresent() ? "True" : "False");
            BackgroundDrawList->AddText(ImVec2(2, 2), IM_COL32(186, 252, 3, 255), buffer);
        }

        if (Menu::bIsOpen)
        {
            ImGui::SetNextWindowSize(ImVec2(500, 300));
            ImGui::Begin("Combat Master Internal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

            if (ImGui::Button("Cheat", ImVec2(100, 20)))
                Menu::currentPage = 0;

            ImGui::SameLine(110);

            if (ImGui::Button("Settings", ImVec2(100, 20)))
                Menu::currentPage = 1;

            if (Menu::currentPage == 0)
            {
                ImGui::Checkbox("ESP (Generic)", &Menu::bEsp);
                ImGui::Checkbox("Break AI's (Host Only)", &Menu::bBreakAi);

                ImGui::Checkbox("Aimbot", &Menu::bAimbot);
                ImGui::Checkbox("Draw FOV", &Menu::bDrawFov);
                ImGui::SliderFloat("Aim FOV", &Menu::aimbotFov, 1.f, 850.f, "%.0f");
                ImGui::SliderFloat("Aim Smooth", &Menu::aimbotSmoothing, 5.f, 20.f, "%.3f");
            }

            else if (Menu::currentPage == 1)
            {
                ImGui::Checkbox("Watermark", &Menu::bWatermark);
                if (ImGui::Button("Log Debug Info"))
                    LogDebugInformation();
            }

            ImGui::End();
        }


        PlayerRoot* localPlayer = PlayerRoot::GetLocalPlayer();

        if (localPlayer)
        {
            Camera* localCamera = localPlayer->GetCamera();
            if (localCamera)
            {
                // shi
                //printf("local camera: %llx | local transform: %llx\n", localCamera, localPlayer->GetNeckTransform());

                ViewMatrix viewMatrix = localCamera->GetViewMatrix();

                PlayerRoot* closestPlayer = ClosestInFOV(Menu::aimbotFov);

                std::vector<PlayerRoot*> entities = GetEntities();
                for (auto& entity : entities)
                {
                    PlayerHealth* playerHealth = entity->GetPlayerHealth();
                    if (!playerHealth) continue;

                    float healthPercent = playerHealth->GetHealthPercent();

                    Vector3 neckPosition = entity->GetNeckPosition();
                    Vector3 rootPosition = entity->GetRootPosition();

                    Vector2 outPos;
                    if (!WorldToScreen(neckPosition, &outPos, viewMatrix))
                        continue;

                    Vector2 outPos2;
                    if (!WorldToScreen(rootPosition, &outPos2, viewMatrix))
                        continue;

                    if (Menu::bBreakAi)
                    {
                        entity->BreakAIBrain();
                    }

                    float distance = Vector3::Distance(localPlayer->GetRootPosition(), neckPosition);

                    ImVec2 headW2s = ImVec2(outPos.x, outPos.y);
                    ImVec2 rootW2s = ImVec2(outPos2.x, outPos2.y);

                    float height = rootW2s.y - headW2s.y;
                    float width = height / 2.0f;

                    ImVec2 boxTopLeft = ImVec2(headW2s.x - width / 2.0f, headW2s.y);
                    ImVec2 boxBottomRight = ImVec2(headW2s.x + width / 2.0f, rootW2s.y);


                    char buffer[38];
                    snprintf(buffer, sizeof(buffer), "PlayerRoot [%im] | %.5f\n", static_cast<int>(distance), healthPercent);
                    BackgroundDrawList->AddLine(ImVec2(Screen::ScreenCenter.x, Screen::ScreenCenter.y), ImVec2(outPos.x, outPos.y), IM_COL32(200, 0, 0, 255));
                    BackgroundDrawList->AddText(ImVec2(outPos.x, outPos.y), IM_COL32(255, 0, 0, 255), buffer);
                    BackgroundDrawList->AddRect(boxTopLeft, boxBottomRight, IM_COL32(255, 0, 0, 255), 0.0f, 0, 1.5f); 
                }

                if (closestPlayer)
                {
                    Vector3 neckPosition = closestPlayer->GetNeckPosition();

                    Vector2 outPos;
                    if (WorldToScreen(neckPosition, &outPos, viewMatrix))
                    {
                        BackgroundDrawList->AddLine(ImVec2(Screen::ScreenCenter.x, Screen::ScreenCenter.y), ImVec2(outPos.x, outPos.y), IM_COL32(0, 255, 255, 255), 2.f);

                        if (GetAsyncKeyState(VK_RBUTTON))
                        {
                            // If player isn't aiming down sights then increase aimbot sensitivity
                            // to prevent erratic movements

                            double posx = (outPos.x - Screen::ScreenCenter.x) / Menu::aimbotSmoothing;
                            double posy = (outPos.y - Screen::ScreenCenter.y) / Menu::aimbotSmoothing;

                            mouse_event(MOUSEEVENTF_MOVE, posx, posy, 0, 0);
                        }
                    }
                }
            }

        }

        ImGui::Render();

        pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        return oPresent(pSwapChain, SyncInterval, Flags);
    }

    bool HookPresent()
    {
        if (!GameOverlayRendererModule) return false;

        uint64_t swapchain = FindSwapchain();
        if (!swapchain) return false;

        uintptr_t address = (uintptr_t)(swapchain); // my overlay 0x150DC0
        Present* steam_present = (Present*)address;
        oPresent = *steam_present;
        _InterlockedExchangePointer((volatile PVOID*)address, hkPresent);

        return true;
    }
}