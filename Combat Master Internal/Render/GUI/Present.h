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

ImU32 GetHealthColor(float healthPercent)
{
    if (healthPercent > 0.75f) return IM_COL32(0, 255, 0, 255);      // Green
    else if (healthPercent > 0.50f) return IM_COL32(255, 255, 0, 255); // Yellow
    else if (healthPercent > 0.25f) return IM_COL32(255, 165, 0, 255); // Orange
    else return IM_COL32(255, 0, 0, 255);                              // Red
}

ImU32 GetDistanceColor(float distance)
{
    if (distance < 25.0f) return IM_COL32(255, 0, 0, 255);        // Close - Red
    else if (distance < 50.0f) return IM_COL32(255, 165, 0, 255); // Medium - Orange
    else if (distance < 100.0f) return IM_COL32(255, 255, 0, 255); // Far - Yellow
    else return IM_COL32(255, 255, 255, 255);                     // Very Far - White
}

void DrawHealthBar(ImDrawList* drawList, ImVec2 position, float width, float height, float healthPercent)
{
    drawList->AddRectFilled(position, ImVec2(position.x + width, position.y + height), IM_COL32(0, 0, 0, 150));

    float healthWidth = width * healthPercent;
    ImU32 healthColor = GetHealthColor(healthPercent);
    drawList->AddRectFilled(position, ImVec2(position.x + healthWidth, position.y + height), healthColor);

    drawList->AddRect(position, ImVec2(position.x + width, position.y + height), IM_COL32(255, 255, 255, 255), 0.0f, 0, 1.0f);
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
            ImGui::SetNextWindowSize(ImVec2(750, 550));
            ImGui::Begin("Combat Master Internal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

            // Modern tab-style navigation with better spacing
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 10));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 0));
            
            // Tab buttons with modern styling
            ImVec2 tabSize = ImVec2(140, 35);
            
            // Combat tab
            if (Menu::currentPage == 0) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.00f, 0.70f, 0.70f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.00f, 0.80f, 0.80f, 0.9f));
                if (ImGui::Button("Combat", tabSize))
                    Menu::currentPage = 0;
                ImGui::PopStyleColor(2);
            } else {
                if (ImGui::Button("Combat", tabSize))
                    Menu::currentPage = 0;
            }
            
            ImGui::SameLine();
            
            // ESP tab
            if (Menu::currentPage == 1) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.00f, 0.70f, 0.70f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.00f, 0.80f, 0.80f, 0.9f));
                if (ImGui::Button("ESP", tabSize))
                    Menu::currentPage = 1;
                ImGui::PopStyleColor(2);
            } else {
                if (ImGui::Button("ESP", tabSize))
                    Menu::currentPage = 1;
            }
            
            ImGui::SameLine();
            
            // Settings tab
            if (Menu::currentPage == 2) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.00f, 0.70f, 0.70f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.00f, 0.80f, 0.80f, 0.9f));
                if (ImGui::Button("Settings", tabSize))
                    Menu::currentPage = 2;
                ImGui::PopStyleColor(2);
            } else {
                if (ImGui::Button("Settings", tabSize))
                    Menu::currentPage = 2;
            }
            
            ImGui::PopStyleVar(2);
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (Menu::currentPage == 0)
            {
                // Combat Features with modern layout
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
                
                // Aimbot Section
                ImGui::BeginChild("AimbotSection", ImVec2(0, 200), true);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.8f, 1.0f));
                ImGui::Text("Aimbot Configuration");
                ImGui::PopStyleColor();
                ImGui::Separator();
                ImGui::Spacing();
                
                ImGui::Checkbox("Enable Aimbot", &Menu::bAimbot);
                ImGui::Spacing();
                
                if (Menu::bAimbot) {
                    ImGui::Indent(20.0f);
                    ImGui::SliderFloat("FOV Range", &Menu::aimbotFov, 1.f, 850.f, "%.0f°");
                    ImGui::SliderFloat("Smoothing", &Menu::aimbotSmoothing, 5.f, 20.f, "%.3f");
                    ImGui::Checkbox("Draw FOV Circle", &Menu::bDrawFov);
                    ImGui::Unindent(20.0f);
                }
                ImGui::EndChild();
                
                ImGui::Spacing();
                
                // Movement & Misc Section
                ImGui::BeginChild("MovementSection", ImVec2(0, 150), true);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.8f, 1.0f));
                ImGui::Text("Movement & Misc");
                ImGui::PopStyleColor();
                ImGui::Separator();
                ImGui::Spacing();
                
                ImGui::Checkbox("Infinite Jump", &Menu::bInfiniteJump);
                ImGui::Checkbox("Break AI's (Host Only)", &Menu::bBreakAi);
                
                ImGui::EndChild();
                
                ImGui::PopStyleVar(2);
            }

            else if (Menu::currentPage == 1)
            {
                // ESP Features with modern layout
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
                
                // Main ESP Toggle
                ImGui::BeginChild("ESPMainSection", ImVec2(0, 80), true);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.8f, 1.0f));
                ImGui::Text("ESP Master Control");
                ImGui::PopStyleColor();
                ImGui::Separator();
                ImGui::Spacing();
                
                ImGui::Checkbox("Enable ESP", &Menu::bEsp);
                ImGui::EndChild();
                
                ImGui::Spacing();
                
                if (Menu::bEsp) {
                    // Visual Elements Section
                    ImGui::BeginChild("ESPVisualSection", ImVec2(0, 180), true);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.8f, 1.0f));
                    ImGui::Text("Visual Elements");
                    ImGui::PopStyleColor();
                    ImGui::Separator();
                    ImGui::Spacing();
                    
                    ImGui::Columns(2, "ESPColumns", false);
                    
                    ImGui::Checkbox("Boxes", &Menu::bEspBoxes);
                    ImGui::Checkbox("Filled Boxes", &Menu::bEspFilledBoxes);
                    ImGui::Checkbox("Snap Lines", &Menu::bEspLines);
                    ImGui::Checkbox("Show Aimbot Target", &Menu::bShowAimbotTarget);
                    
                    ImGui::NextColumn();
                    
                    ImGui::Checkbox("Player Names", &Menu::bEspNames);
                    ImGui::Checkbox("Health Text", &Menu::bEspHealth);
                    ImGui::Checkbox("Health Bars", &Menu::bEspHealthBar);
                    ImGui::Checkbox("Distance Info", &Menu::bEspDistance);
                    
                    ImGui::Columns(1);
                    
                    ImGui::EndChild();
                    
                    ImGui::Spacing();
                    
                    // ESP Settings Section
                    ImGui::BeginChild("ESPSettingsSection", ImVec2(0, 100), true);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.8f, 1.0f));
                    ImGui::Text("ESP Settings");
                    ImGui::PopStyleColor();
                    ImGui::Separator();
                    ImGui::Spacing();
                    
                    ImGui::SliderFloat("Max Distance", &Menu::espMaxDistance, 10.f, 500.f, "%.0fm");
                    
                    ImGui::EndChild();
                }
                
                ImGui::PopStyleVar(2);
            }

            else if (Menu::currentPage == 2)
            {
                // Settings with modern layout
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
                
                // UI Settings Section
                ImGui::BeginChild("UISettingsSection", ImVec2(0, 120), true);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.8f, 1.0f));
                ImGui::Text("Interface Settings");
                ImGui::PopStyleColor();
                ImGui::Separator();
                ImGui::Spacing();
                
                ImGui::Checkbox("Show Watermark", &Menu::bWatermark);
                
                ImGui::Spacing();
                ImGui::Text("Menu Toggle: INSERT key");
                
                ImGui::EndChild();
                
                ImGui::Spacing();
                
                // Information Section
                ImGui::BeginChild("InfoSection", ImVec2(0, 200), true);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.8f, 1.0f));
                ImGui::Text("Information");
                ImGui::PopStyleColor();
                ImGui::Separator();
                ImGui::Spacing();
                
                ImGui::Text("Combat Master Internal");
                ImGui::Text("Version: 1.0");
                ImGui::Spacing();
                
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                ImGui::TextWrapped("Features:");
                ImGui::BulletText("Advanced Aimbot with FOV control");
                ImGui::BulletText("Comprehensive ESP system");
                ImGui::BulletText("Movement enhancements");
                ImGui::BulletText("AI manipulation tools");
                ImGui::PopStyleColor();
                
                ImGui::EndChild();
                
                ImGui::PopStyleVar(2);
            }

            ImGui::End();
        }


        PlayerRoot* localPlayer = PlayerRoot::GetLocalPlayer();

        if (localPlayer)
        {
            if (Menu::bInfiniteJump) {
                PlayerMovement* movement = localPlayer->GetPlayerMovement();
                if (movement) 
                {
                    *(bool*)((DWORD64)movement + 0x94) = false; // isInAir
                    *(bool*)((DWORD64)movement + 0x96) = false; // isFalling
                }
            }
            Camera* localCamera = localPlayer->GetCamera();
            if (localCamera)
            {
                ViewMatrix viewMatrix = localCamera->GetViewMatrix();
                PlayerRoot* closestPlayer = ClosestInFOV(Menu::aimbotFov);

                std::vector<PlayerRoot*> entities = GetEntities();

                for (auto& entity : entities)
                {
                    PlayerHealth* playerHealth = entity->GetPlayerHealth();
                    if (!playerHealth) continue;

                    float healthPercent = playerHealth ? playerHealth->GetHealthPercent() : 0.0f;
                    bool isDead = entity->IsDead();
                    bool isTeammate = entity->IsTeammate();

                    Vector3 neckPosition = entity->GetNeckPosition();
                    Vector3 rootPosition = entity->GetRootPosition();
                    float distance = Vector3::Distance(localPlayer->GetRootPosition(), neckPosition);

                    if (distance > Menu::espMaxDistance) continue;

                    Vector2 outPos, outPos2;
                    if (!WorldToScreen(neckPosition, &outPos, viewMatrix)) continue;
                    if (!WorldToScreen(rootPosition, &outPos2, viewMatrix)) continue;

                    if (Menu::bBreakAi)
                    {
                        entity->BreakAIBrain();
                    }

                    ImVec2 headW2s = ImVec2(outPos.x, outPos.y);
                    ImVec2 rootW2s = ImVec2(outPos2.x, outPos2.y);
                    float height = rootW2s.y - headW2s.y;
                    float width = height / 2.0f;
                    ImVec2 boxTopLeft = ImVec2(headW2s.x - width / 2.0f, headW2s.y);
                    ImVec2 boxBottomRight = ImVec2(headW2s.x + width / 2.0f, rootW2s.y);

                    ImU32 mainColor = IM_COL32(255, 255, 255, 255);
                    if (isTeammate) {
                        mainColor = IM_COL32(0, 255, 0, 255);
                    }
                    else {
                        mainColor = IM_COL32(255, 0, 0, 255);
                    }

                    if (Menu::bEsp)
                    {
                        if (Menu::bEspBoxes) {
                            if (Menu::bEspFilledBoxes) {
                                BackgroundDrawList->AddRectFilled(boxTopLeft, boxBottomRight, IM_COL32(mainColor & 0xFF, (mainColor >> 8) & 0xFF, (mainColor >> 16) & 0xFF, 50));
                            }
                            BackgroundDrawList->AddRect(boxTopLeft, boxBottomRight, mainColor, 0.0f, 0, 1.5f);
                        }
                        if (Menu::bEspLines) {
                            BackgroundDrawList->AddLine(ImVec2(Screen::ScreenCenter.x, Screen::ScreenCenter.y), ImVec2(outPos.x, outPos.y), mainColor, 1.0f);
                        }

                        float textYOffset = 0.0f;
                        const float textSpacing = 15.0f;

                        if (Menu::bEspNames) {
                            char nameBuffer[64];
                            snprintf(nameBuffer, sizeof(nameBuffer), "Player_%llx", (uint64_t)entity);
                            BackgroundDrawList->AddText(ImVec2(boxBottomRight.x + 5, boxTopLeft.y + textYOffset), mainColor, nameBuffer);
                            textYOffset += textSpacing;
                        }

                        if (Menu::bEspHealth && playerHealth) {
                            char healthBuffer[32];
                            snprintf(healthBuffer, sizeof(healthBuffer), "HP: %.0f%%", healthPercent * 100.0f);
                            BackgroundDrawList->AddText(ImVec2(boxBottomRight.x + 5, boxTopLeft.y + textYOffset), GetHealthColor(healthPercent), healthBuffer);
                            textYOffset += textSpacing;
                        }

                        if (Menu::bEspHealthBar && playerHealth) {
                            ImVec2 healthBarPos = ImVec2(boxTopLeft.x - 8, boxTopLeft.y);
                            DrawHealthBar(BackgroundDrawList, healthBarPos, 4, height, healthPercent);
                        }

                        if (Menu::bEspDistance) {
                            char distanceBuffer[32];
                            snprintf(distanceBuffer, sizeof(distanceBuffer), "%.0fm", distance);
                            BackgroundDrawList->AddText(ImVec2(boxBottomRight.x + 5, boxTopLeft.y + textYOffset), mainColor, distanceBuffer);
                            textYOffset += textSpacing;
                        }
                        if (isTeammate) {
                            BackgroundDrawList->AddText(ImVec2(boxBottomRight.x + 5, boxTopLeft.y + textYOffset), IM_COL32(0, 255, 0, 255), "TEAMMATE");
                        }
                    }
                }

                if (closestPlayer)
                {
                    Vector3 neckPosition = closestPlayer->GetNeckPosition();

                    Vector2 outPos;
                    if (WorldToScreen(neckPosition, &outPos, viewMatrix))
                    {
                        if (Menu::bShowAimbotTarget) {
                            BackgroundDrawList->AddLine(ImVec2(Screen::ScreenCenter.x, Screen::ScreenCenter.y), ImVec2(outPos.x, outPos.y), IM_COL32(0, 255, 255, 255), 2.f);
                        }
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