#pragma once

namespace GUI
{
    void InitMenuStyle(ID3D11Device* pDevice)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowRounding = 9.f;
        style.Colors[ImGuiCol_WindowBg] = ImColor(30, 30, 30, 250);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.f, 0.f, 0.f, 0.f); // Set to invisible for custom text input boxes
        style.Colors[ImGuiCol_CheckMark] = ImColor(200, 243, 30);
        style.Colors[ImGuiCol_SliderGrabActive] = ImColor(200, 243, 30);
        style.Colors[ImGuiCol_SliderGrab] = ImColor(200, 243, 30);
        style.Colors[ImGuiCol_Button] = ImColor(40, 40, 40, 250);
        style.Colors[ImGuiCol_ButtonHovered] = ImColor(45, 45, 45, 250);
        style.Colors[ImGuiCol_ButtonActive] = ImColor(40, 40, 40, 250);
        style.Colors[ImGuiCol_FrameBg] = ImColor(40, 40, 40, 250);
        style.Colors[ImGuiCol_FrameBgHovered] = ImColor(45, 45, 45, 250);
        style.Colors[ImGuiCol_FrameBgActive] = ImColor(45, 45, 45, 250);
        style.Colors[ImGuiCol_Border] = ImColor(37, 37, 37, 255);
    }
}