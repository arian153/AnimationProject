#include "GameEditor.hpp"
#include "../../../External/imgui/imgui.h"
#include "../../Core/OS-API/Application.hpp"
#include "../../../Manager/Level/LevelManager.hpp"
#include "../../../Manager/Object/ObjectFactory.hpp"
#include "../../../Manager/Object/ObjectManager.hpp"
#include "../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../Manager/Space/Space.hpp"
#include "Command/EditorCommand.hpp"

namespace CS460
{
    GameEditor::GameEditor()
    {
    }

    GameEditor::~GameEditor()
    {
    }

    void GameEditor::Initialize(Application* application)
    {
        m_application                = application;
        m_space_editor.m_game_editor = this;
        m_space_editor.Initialize(application);
        m_window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        m_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        m_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        m_command_registry.Initialize();
        m_space_editor.m_command_registry = &m_command_registry;

        m_object_factory   = m_application->GetObjectFactory();
        m_resource_manager = m_application->GetResourceManager();
        m_object_factory->GetArchetypeName(m_archetype_names);
    }

    void GameEditor::Update(Real dt)
    {
        if (m_b_open)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Engine 5th Editor", nullptr, m_window_flags);
            ImGui::PopStyleVar(3);
            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dock_space_id = ImGui::GetID("E5-Editor");
                ImGui::DockSpace(dock_space_id, ImVec2(0.0f, 0.0f), m_dock_space_flags);
            }
            if (ImGui::BeginMenuBar())
            {
                UpdateFileTab();
                UpdateEditTab();
                UpdateObjectTab();
                ImGui::EndMenuBar();
            }
            ImGui::End();
            {
                m_space_editor.UpdateSceneWindow(dt);
                m_space_editor.UpdateHierarchyWindow();
                m_space_editor.UpdateInspectorWindow();
                m_space_editor.UpdateSpaceSetting();
                m_level_editor.Update(dt);
                UpdateCommandWindow();
            }

            ShowReadMe();
        }
    }

    void GameEditor::Shutdown()
    {
        m_command_registry.Shutdown();
    }

    bool GameEditor::IsOpen() const
    {
        return m_b_open;
    }

    void GameEditor::SetOpen(bool is_open)
    {
        m_b_open = is_open;
    }

    void GameEditor::UpdateFileTab()
    {
        if (ImGui::BeginMenu("File"))
        {
            size_t open_count = m_space_editor.OpenCount();
            if (ImGui::BeginMenu("New"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Open"))
            {
                if (ImGui::BeginMenu("Space", open_count < m_space_editor.Size()))
                {
                    m_space_editor.OpenSequence();
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Close", nullptr, false, open_count > 0))
            {
                m_space_editor.CloseSequence();
            }
            if (ImGui::MenuItem("Close All", nullptr, false, open_count > 0))
            {
                m_space_editor.CloseAllSequence();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save", nullptr, false, open_count > 0))
            {
            }
            if (ImGui::MenuItem("Save All", nullptr, false, open_count > 0))
            {
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                m_application->GetLevelManager()->SetQuit();
            }
            ImGui::EndMenu();
        }
    }

    void GameEditor::UpdateEditTab()
    {
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z"))
            {
                m_command_registry.UndoCommand();
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y"))
            {
                m_command_registry.RedoCommand();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X"))
            {
            }
            if (ImGui::MenuItem("Copy", "Ctrl+C"))
            {
            }
            if (ImGui::MenuItem("Paste", "Ctrl+V"))
            {
            }
            if (ImGui::MenuItem("Duplicate", "Ctrl+D"))
            {
            }
            if (ImGui::MenuItem("Delete", "Ctrl+R"))
            {
            }
            ImGui::EndMenu();
        }
    }

    void GameEditor::UpdateObjectTab()
    {
        if (ImGui::BeginMenu("Object"))
        {
            /*if (ImGui::MenuItem("Create Empty"))
             {
             }*/
            if (ImGui::BeginMenu("Create 3D Object"))
            {
                size_t size = m_archetype_names.size();

                for (size_t i = 0; i < size; ++i)
                {
                    if (ImGui::MenuItem(("Create " + m_archetype_names[i]).c_str()))
                    {
                        if (m_space_editor.m_editing_space != nullptr)
                        {
                            m_space_editor.m_editing_space->GetObjectManager()->AddObject(m_archetype_names[i], i);
                        }
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            /*if (ImGui::MenuItem("Create Camera"))
              {
              }
              if (ImGui::MenuItem("Create Light"))
              {
              }*/
            ImGui::EndMenu();
        }
    }

    static bool Items_CommandGetter(void* data, int idx, const char** out_text)
    {
        auto& commands = *static_cast<std::vector<EditorCommand*>*>(data);
        if (idx < 0 || idx >= static_cast<int>(commands.size()))
        {
            return false;
        }
        *out_text = commands[idx]->TypeCStr();
        return true;
    }

    void GameEditor::UpdateCommandWindow()
    {
        ImGui::Begin("Commands");
        ImVec2 min = ImGui::GetWindowContentRegionMin();
        ImVec2 max = ImGui::GetWindowContentRegionMax();
        ImGui::PushItemWidth(max.x - min.x);
        auto commands = m_command_registry.m_command_registry;
        if (ImGui::ListBox(
                           "##ObjectList", &m_command_index, Items_CommandGetter,
                           static_cast<void*>(&commands), static_cast<int>(commands.size()), (int)m_command_registry.m_registry_size))
        {
        }
        ImGui::End();
    }

    void GameEditor::ShowReadMe()
    {
        if (m_show_readme)
        {
            if (ImGui::Begin("Read Me", &m_show_readme))
            {
                ImGui::Text("Usage");
                ImGui::Text("1. Click File Tab, and Open Space.");
                ImGui::Text("2. Camera Control");
                ImGui::Text("    Hold Mouse RB & move - Change camera view direction on spherically");
                ImGui::Text("    Roll Mouse Wheel     - Change spherical camera's radius on same view");
                ImGui::Text("    Press W, S           - Change camera position Front & Back");
                ImGui::Text("    Press A, D           - Change camera position Left & Right");
                ImGui::Text("    Press R, F           - Change camera position Up & Down");
                ImGui::Text("3. Select Object in Hierarchy Window");
                ImGui::Text("4. Open Component detail in Inspector Window");

                ImGui::NewLine();
                if (ImGui::Button("  Close  "))
                {
                    m_show_readme = false;
                }
                ImGui::End();
            }
        }
    }
}
