#pragma once

namespace GUI
{
    void InitMenuStyle(ID3D11Device* pDevice)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

        ImGuiStyle& style = ImGui::GetStyle();

        // Modern rounded corners and spacing
        style.WindowRounding = 12.0f;
        style.ChildRounding = 8.0f;
        style.FrameRounding = 6.0f;
        style.PopupRounding = 8.0f;
        style.ScrollbarRounding = 12.0f;
        style.GrabRounding = 6.0f;
        style.TabRounding = 6.0f;
        
        // Enhanced padding and spacing
        style.WindowPadding = ImVec2(15, 15);
        style.FramePadding = ImVec2(8, 6);
        style.ItemSpacing = ImVec2(8, 6);
        style.ItemInnerSpacing = ImVec2(6, 4);
        style.IndentSpacing = 25.0f;
        style.ScrollbarSize = 14.0f;
        style.GrabMinSize = 12.0f;
        
        // Modern color scheme - Dark theme with accent colors
        ImVec4* colors = style.Colors;
        
        // Main window colors
        colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.10f, 0.95f);  // Dark background
        colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.90f);   // Slightly lighter child windows
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.10f, 0.95f);
        
        // Text colors
        colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);      // Light text
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        
        // Frame/Input colors
        colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.18f, 0.90f);   // Input backgrounds
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.25f, 0.90f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.30f, 0.90f);
        
        // Button colors - Modern blue accent
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.35f, 0.90f);    // Dark blue
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.40f, 0.60f, 0.90f); // Brighter blue
        colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.20f, 0.30f, 0.90f);  // Darker when pressed
        
        // Header colors (for collapsing headers)
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.35f, 0.70f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.40f, 0.60f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.35f, 0.50f, 0.80f);
        
        // Accent color for checkmarks, sliders, etc. - Cyan/Teal accent
        colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.80f, 0.80f, 1.00f);  // Cyan
        colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.90f, 0.90f, 1.00f);
        
        // Scrollbar colors
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.07f, 0.90f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.30f, 0.90f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.40f, 0.90f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.45f, 0.45f, 0.50f, 0.90f);
        
        // Border colors
        colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.30f, 0.80f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        
        // Tab colors
        colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.20f, 0.90f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.40f, 0.60f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.35f, 0.90f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.15f, 0.90f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.20f, 0.90f);
        
        // Title bar
        colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.95f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.20f, 0.30f, 0.95f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.10f, 0.75f);
        
        // Separator
        colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.30f, 0.80f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.30f, 0.40f, 0.60f, 0.80f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);
        
        // Resize grip
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.25f, 0.25f, 0.30f, 0.50f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.30f, 0.40f, 0.60f, 0.70f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.70f, 0.70f, 0.90f);
        
        // Plot colors
        colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 0.80f, 0.80f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.20f, 0.60f, 0.80f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.30f, 0.70f, 0.90f, 1.00f);
        
        // Table colors
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.03f);
        
        // Selection colors
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.70f, 0.70f, 0.35f);
        
        // Drag and drop
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 0.80f, 0.80f, 0.90f);
        
        // Navigation
        colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 0.80f, 0.80f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        
        // Modal window dim background
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
    }
}