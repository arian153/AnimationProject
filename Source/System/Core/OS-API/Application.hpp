#pragma once
#include "ApplicationSetting.hpp"

namespace CS460
{
    class GameEditor;
    class GUISystem;
    class LogicSystem;
    class FileUtility;
    class ResourceManager;
    class InputCommon;
    class ObjectFactory;
    class ComponentRegistry;
    class RenderSystem;
    class PhysicsSystem;
    class OSCommon;
    class TimeUtility;
    class LevelManager;
    class SpaceManager;
    class FrameUtility;

    class Application
    {
    public:
        Application();
        ~Application();

        void Initialize();
        void Update() const;
        void Shutdown();

        //subsystem getter
        Application*       GetApplication();
        OSCommon*          GetOperatingSystem() const;
        RenderSystem*      GetRenderSystem() const;
        PhysicsSystem*     GetPhysicsSystem() const;
        LogicSystem*       GetLogicSystem() const;
        FrameUtility*      GetFrameUtility() const;
        TimeUtility*       GetApplicationTimer() const;
        LevelManager*      GetLevelManager() const;
        SpaceManager*      GetSpaceManager() const;
        ObjectFactory*     GetObjectFactory() const;
        ComponentRegistry* GetComponentRegistry() const;
        InputCommon*       GetInput() const;
        FileUtility*       GetFileUtility() const;
        ResourceManager*   GetResourceManager() const;
        GUISystem*         GetGUISystem() const;
        GameEditor*        GetGameEditor() const;

    private:
        void OnResize(int client_width, int client_height) const;
        void OnFullscreen() const;
        void OnQuit();
        void LoadSetting();

    private:
        friend class OSCommon;
        friend class LevelManager;

    private:
        ApplicationSetting m_initial_setting;

    private:
        //sub systems
        OSCommon*          m_operating_system   = nullptr;
        RenderSystem*      m_render_system      = nullptr;
        PhysicsSystem*     m_physics_system     = nullptr;
        LogicSystem*       m_logic_system       = nullptr;
        GUISystem*         m_gui_system         = nullptr;
        TimeUtility*       m_time_utility       = nullptr;
        FrameUtility*      m_frame_utility      = nullptr;
        LevelManager*      m_level_manager      = nullptr;
        SpaceManager*      m_space_manager      = nullptr;
        ObjectFactory*     m_object_factory     = nullptr;
        ComponentRegistry* m_component_registry = nullptr;
        InputCommon*       m_input              = nullptr;
        FileUtility*       m_file_utility       = nullptr;
        ResourceManager*   m_resource_manager   = nullptr;
        GameEditor*        m_game_editor        = nullptr;
    };
}
