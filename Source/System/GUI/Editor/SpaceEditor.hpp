#pragma once
#include "../../../External/imgui/imgui.h"
#include <vector>
#include "../../Math/Utility/MathDef.hpp"
#include <unordered_map>

#include "../../Math/Algebra/Vector2.hpp"
#include "../../Math/Primitive/Others/Ray.hpp"

namespace CS460
{
    class FrameUtility;
    class CommandRegistry;
    class Object;
    class GUISystem;
    class JsonResource;
    class ResourceManager;
    class SpaceManager;
    class Application;
    class RenderTextureGenerator;
    class Space;

    class SpaceEditor
    {
    public:
        SpaceEditor();
        ~SpaceEditor();

        void Initialize(Application* application);
        void SetStepFrameTime(Real target_frame_per_second);

    private:
        void UpdateSceneWindow(Real dt);
        void UpdateInspectorWindow();
        void UpdateHierarchyWindow();
        void UpdateSpaceSetting();

        void OpenSequence();
        void CloseAllSequence();
        void CloseSequence();

        size_t OpenCount() const;
        size_t Size() const;

    private:
        void   DoOpen(JsonResource* resource);
        void   DoQueueClose(JsonResource* resource);
        void   DoForceClose(JsonResource* resource);
        void   DoSave(JsonResource* resource);
        void   DisplayContents(JsonResource* resource);
        void   DisplayContextMenu(JsonResource* resource);
        Space* DisplayScene(const std::string& name, Real dt);

    private:
        friend class GameEditor;

    private:
        bool             m_b_reorderable = true;
        ImGuiTabBarFlags m_fitting_flags = ImGuiTabBarFlags_FittingPolicyDefault_;
        ImVec2           m_uv_min        = ImVec2(0.0f, 0.0f);                   // Top-left
        ImVec2           m_uv_max        = ImVec2(1.0f, 1.0f);                   // Lower-right
        ImVec4           m_tint_col      = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
        ImVec4           m_border_col    = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
        ImVec2           m_mouse_pos     = ImVec2(0.0f, 0.0f);
        Vector2          m_ortho_pos;
        Ray              m_picking_ray;

    private:
        GameEditor*      m_game_editor      = nullptr;
        Application*     m_application      = nullptr;
        SpaceManager*    m_space_manager    = nullptr;
        ResourceManager* m_resource_manager = nullptr;
        Space*           m_editing_space    = nullptr;
        Object*          m_editing_object   = nullptr;
        CommandRegistry* m_command_registry = nullptr;
        FrameUtility*    m_frame_utility    = nullptr;
        //tool
        std::vector<JsonResource*>              m_resources;
        std::vector<JsonResource*>              m_close_queue;
        std::unordered_map<std::string, Space*> m_editing_spaces;

        RenderTextureGenerator* m_render_texture_generator = nullptr;

        bool   m_b_pause      = false;
        bool   m_b_step       = false;
        Real   m_time_step    = 1.0f / 60.0f;
        size_t m_space_index  = 0;
        int    m_object_index = 0;
    };
}
