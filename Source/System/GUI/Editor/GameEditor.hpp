#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "LevelEditor.hpp"
#include "SpaceEditor.hpp"
#include "../../../External/imgui/imgui.h"
#include "Command/CommandRegistry.hpp"

namespace CS460
{
    class ObjectFactory;
    class GUISystem;
    class Application;

    class GameEditor
    {
    public:
        GameEditor();
        ~GameEditor();

        void Initialize(Application* application);
        void Update(Real dt);
        void Shutdown();

        bool IsOpen() const;
        void SetOpen(bool is_open);

    private:
        void UpdateFileTab();
        void UpdateEditTab();
        void UpdateObjectTab();
        void UpdateCommandWindow();
        void ShowReadMe();

    private:
        LevelEditor     m_level_editor;
        SpaceEditor     m_space_editor;
        Application*    m_application = nullptr;
        CommandRegistry m_command_registry;

        ObjectFactory*   m_object_factory   = nullptr;
        ResourceManager* m_resource_manager = nullptr;

        std::vector<std::string> m_archetype_names;

        //editor flag
        bool        m_b_open        = true;
        std::string m_editor_label  = "";
        int         m_command_index = 0;

        bool m_show_readme = true;

        //imgui flag
        ImGuiDockNodeFlags m_dock_space_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags   m_window_flags     = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    };
}
