#include "GUISystem.hpp"
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/win32/imgui_impl_win32.h"
#include "../../External/imgui/dx11/imgui_impl_dx11.h"
#include "../Core/OS-API/Application.hpp"
#include "../Core/OS-API/OSCommon.hpp"
#include "../Graphics/RenderSystem.hpp"
#include "../Graphics/Common/Renderer/RendererCommon.hpp"
#include "Editor/GameEditor.hpp"

namespace CS460
{
    GUISystem::GUISystem(ImGuiIO& io)
        : m_im_gui_io(io)
    {
    }

    GUISystem::~GUISystem()
    {
    }

    void GUISystem::Initialize(Application* application)
    {
        (void)m_im_gui_io;
        //m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
        m_im_gui_io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI
        // Setup Dear ImGui style
        StyleVisualStudio();
        //ImGui::StyleColorsLight();
        //ImGui::StyleColorsClassic();
        ImGuiStyle& style    = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
#if defined(CS460_WIN32)
        ImGui_ImplWin32_Init(application->GetOperatingSystem()->AppHWnd());
#endif
#if defined(CS460_DIRECTX11)
        m_renderer = application->GetRenderSystem()->GetRenderer();
        ImGui_ImplDX11_Init(m_renderer->GetDevice(), m_renderer->GetDeviceContext());
#endif
    }

    void GUISystem::Shutdown()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void GUISystem::Update(Real dt)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        m_game_editor->Update(dt);
    }

    void GUISystem::BeginRender() const
    {
        ImGui::Render();
        m_renderer->SetBackBufferRenderTarget();
    }

    void GUISystem::EndRender() const
    {
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    bool GUISystem::IsFocusGUI()
    {
        if (m_b_free_focus)
            return false;
        return ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);
    }

    void GUISystem::CreateGUIContext()
    {
#if defined(E5_WIN32)
        ImGui_ImplWin32_EnableDpiAwareness();
#endif
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
    }

    void GUISystem::SetFocusFree(bool flag)
    {
        m_b_free_focus = flag;
    }

    void GUISystem::AddGUI(GameEditor* game_editor)
    {
        m_game_editor = game_editor;
    }

    void GUISystem::StyleVisualStudio()
    {
        constexpr auto ColorFromBytes = [](uint8_t r, uint8_t g, uint8_t b)
        {
            return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
        };

        auto&   style  = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        const ImVec4 bg_color            = ColorFromBytes(37, 37, 38);
        const ImVec4 light_bg_color      = ColorFromBytes(82, 82, 85);
        const ImVec4 very_light_bg_color = ColorFromBytes(90, 90, 95);

        const ImVec4 panel_color        = ColorFromBytes(51, 51, 55);
        const ImVec4 panel_hover_color  = ColorFromBytes(29, 151, 236);
        const ImVec4 panel_active_color = ColorFromBytes(0, 119, 200);

        const ImVec4 text_color          = ColorFromBytes(255, 255, 255);
        const ImVec4 text_disabled_color = ColorFromBytes(151, 151, 151);
        const ImVec4 border_color        = ColorFromBytes(78, 78, 78);

        colors[ImGuiCol_Text]                 = text_color;
        colors[ImGuiCol_TextDisabled]         = text_disabled_color;
        colors[ImGuiCol_TextSelectedBg]       = panel_active_color;
        colors[ImGuiCol_WindowBg]             = bg_color;
        colors[ImGuiCol_ChildBg]              = bg_color;
        colors[ImGuiCol_PopupBg]              = bg_color;
        colors[ImGuiCol_Border]               = border_color;
        colors[ImGuiCol_BorderShadow]         = border_color;
        colors[ImGuiCol_FrameBg]              = panel_color;
        colors[ImGuiCol_FrameBgHovered]       = panel_hover_color;
        colors[ImGuiCol_FrameBgActive]        = panel_active_color;
        colors[ImGuiCol_TitleBg]              = bg_color;
        colors[ImGuiCol_TitleBgActive]        = bg_color;
        colors[ImGuiCol_TitleBgCollapsed]     = bg_color;
        colors[ImGuiCol_MenuBarBg]            = panel_color;
        colors[ImGuiCol_ScrollbarBg]          = panel_color;
        colors[ImGuiCol_ScrollbarGrab]        = light_bg_color;
        colors[ImGuiCol_ScrollbarGrabHovered] = very_light_bg_color;
        colors[ImGuiCol_ScrollbarGrabActive]  = very_light_bg_color;
        colors[ImGuiCol_CheckMark]            = panel_active_color;
        colors[ImGuiCol_SliderGrab]           = panel_hover_color;
        colors[ImGuiCol_SliderGrabActive]     = panel_active_color;
        colors[ImGuiCol_Button]               = panel_color;
        colors[ImGuiCol_ButtonHovered]        = panel_hover_color;
        colors[ImGuiCol_ButtonActive]         = panel_hover_color;
        colors[ImGuiCol_Header]               = panel_color;
        colors[ImGuiCol_HeaderHovered]        = panel_hover_color;
        colors[ImGuiCol_HeaderActive]         = panel_active_color;
        colors[ImGuiCol_Separator]            = border_color;
        colors[ImGuiCol_SeparatorHovered]     = border_color;
        colors[ImGuiCol_SeparatorActive]      = border_color;
        colors[ImGuiCol_ResizeGrip]           = bg_color;
        colors[ImGuiCol_ResizeGripHovered]    = panel_color;
        colors[ImGuiCol_ResizeGripActive]     = light_bg_color;
        colors[ImGuiCol_PlotLines]            = panel_active_color;
        colors[ImGuiCol_PlotLinesHovered]     = panel_hover_color;
        colors[ImGuiCol_PlotHistogram]        = panel_active_color;
        colors[ImGuiCol_PlotHistogramHovered] = panel_hover_color;
        colors[ImGuiCol_ModalWindowDarkening] = bg_color;
        colors[ImGuiCol_DragDropTarget]       = bg_color;
        colors[ImGuiCol_NavHighlight]         = bg_color;
        colors[ImGuiCol_DockingPreview]       = panel_active_color;
        colors[ImGuiCol_Tab]                  = bg_color;
        colors[ImGuiCol_TabActive]            = panel_active_color;
        colors[ImGuiCol_TabUnfocused]         = bg_color;
        colors[ImGuiCol_TabUnfocusedActive]   = panel_active_color;
        colors[ImGuiCol_TabHovered]           = panel_hover_color;

        style.WindowRounding    = 0.0f;
        style.ChildRounding     = 0.0f;
        style.FrameRounding     = 0.0f;
        style.GrabRounding      = 0.0f;
        style.PopupRounding     = 0.0f;
        style.ScrollbarRounding = 0.0f;
        style.TabRounding       = 0.0f;
    }
}
