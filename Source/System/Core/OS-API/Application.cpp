#include "Application.hpp"
#include "OSCommon.hpp"

//systems
#include "../../Graphics/RenderSystem.hpp"
#include "../../Physics/PhysicsSystem.hpp"

//utilities
#include "../Utility/TimeUtility.hpp"
#include <string>
#include "../../../Manager/Object/ObjectFactory.hpp"
#include "../../../Manager/Component/ComponentRegistry.hpp"
#include "../../../Manager/Space/SpaceManager.hpp"
#include "../../../Manager/Level/LevelManager.hpp"
#include "../Utility/FrameUtility.hpp"
#include "../Input/InputCommon.hpp"
#include "../Utility/FileUtility.hpp"
#include "../../../Manager/Resource/ResourceManager.hpp"
#include "../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../Animation/AnimationSystem.hpp"
#include "../Utility/ConsoleUtility.hpp"
#include "../../Logic/LogicSystem.hpp"
#include "../../GUI/GUISystem.hpp"
#include "../../GUI/Editor/GameEditor.hpp"

namespace CS460
{
    Application::Application()
        : m_initial_setting()
    {
    }

    Application::~Application()
    {
    }

    void Application::Initialize()
    {
        //create independent utility
        m_time_utility     = new TimeUtility();
        m_frame_utility    = new FrameUtility();
        m_file_utility     = new FileUtility();
        m_resource_manager = new ResourceManager();
        m_resource_manager->Initialize(m_file_utility);
        LoadSetting();
        //create systems
        m_operating_system = new OSCommon(this);
        m_operating_system->Initialize();
        m_input = new InputCommon();
        m_input->Initialize();
        m_render_system = new RenderSystem(m_operating_system);
        m_render_system->Initialize(m_initial_setting.screen_width, m_initial_setting.screen_height, m_initial_setting.b_v_sync, m_resource_manager);
        m_physics_system = new PhysicsSystem();
        m_physics_system->Initialize();
        //GUI
        GUISystem::CreateGUIContext();
        m_gui_system = new GUISystem(ImGui::GetIO());
        m_gui_system->Initialize(this);
        //Logic
        m_logic_system = new LogicSystem();
        m_logic_system->SetInput(m_input);
        m_logic_system->SetTextRenderer(m_render_system->GetTextRenderer());
        m_logic_system->Initialize();

        m_animation_system = new AnimationSystem();
        m_animation_system->SetAppUtility(m_time_utility, m_frame_utility);
        m_animation_system->Initialize();

        //create managers
        m_component_registry = new ComponentRegistry();
        m_component_registry->Initialize();
        m_object_factory = new ObjectFactory();
        m_object_factory->Initialize(m_component_registry);

        m_space_manager = new SpaceManager(
                                           m_physics_system,
                                           m_render_system,
                                           m_object_factory,
                                           m_component_registry,
                                           m_resource_manager,
                                           m_logic_system,
                                           m_animation_system);
        m_space_manager->Initialize();

        m_object_factory->LoadArchetype(m_resource_manager);

        m_level_manager = new LevelManager();
        m_level_manager->Initialize(this);
        //setup missing system parameters
        m_operating_system->SetLevelManager(m_level_manager);
        m_operating_system->SetInput(m_input);
        //Console::CreateConsole();
        m_game_editor = new GameEditor();
        m_game_editor->Initialize(this);
        m_gui_system->AddGUI(m_game_editor);
        m_level_manager->SetInitialLevel(m_initial_setting.initial_level);
        m_game_editor->SetOpen(m_initial_setting.initial_level == "");
    }

    void Application::Update() const
    {
        while (m_operating_system->IsQuit() == false && m_operating_system->IsInit() == true)
        {
            m_level_manager->Update();
        }
    }

    void Application::Shutdown()
    {
        if (m_game_editor != nullptr)
        {
            m_game_editor->Shutdown();
            delete m_game_editor;
            m_game_editor = nullptr;
        }
        if (m_level_manager != nullptr)
        {
            m_level_manager->Shutdown();
            delete m_level_manager;
            m_level_manager = nullptr;
        }
        if (m_object_factory != nullptr)
        {
            m_object_factory->Shutdown();
            delete m_object_factory;
            m_object_factory = nullptr;
        }

        if (m_space_manager != nullptr)
        {
            m_space_manager->Shutdown();
            delete m_space_manager;
            m_space_manager = nullptr;
        }

        if (m_component_registry != nullptr)
        {
            m_component_registry->Shutdown();
            delete m_component_registry;
            m_component_registry = nullptr;
        }
        if (m_resource_manager != nullptr)
        {
            m_resource_manager->Shutdown();
            delete m_resource_manager;
            m_resource_manager = nullptr;
        }
        if (m_file_utility != nullptr)
        {
            delete m_file_utility;
            m_file_utility = nullptr;
        }
        if (m_time_utility != nullptr)
        {
            delete m_time_utility;
            m_time_utility = nullptr;
        }
        if (m_frame_utility != nullptr)
        {
            delete m_frame_utility;
            m_frame_utility = nullptr;
        }
        if (m_gui_system != nullptr)
        {
            m_gui_system->Shutdown();
            delete m_gui_system;
            m_gui_system = nullptr;
        }
        if (m_logic_system != nullptr)
        {
            m_logic_system->Shutdown();
            delete m_logic_system;
            m_logic_system = nullptr;
        }

        if (m_animation_system != nullptr)
        {
            m_animation_system->Shutdown();
            delete m_animation_system;
            m_animation_system = nullptr;
        }
        if (m_physics_system != nullptr)
        {
            m_physics_system->Shutdown();
            delete m_physics_system;
            m_physics_system = nullptr;
        }
        if (m_render_system != nullptr)
        {
            m_render_system->Shutdown();
            delete m_render_system;
            m_render_system = nullptr;
        }
        if (m_input != nullptr)
        {
            m_input->Shutdown();
            delete m_input;
            m_input = nullptr;
        }
        if (m_operating_system != nullptr)
        {
            m_operating_system->Shutdown();
            delete m_operating_system;
            m_operating_system = nullptr;
        }
        //Console::DestroyConsole();
    }

    Application* Application::GetApplication()
    {
        return this;
    }

    OSCommon* Application::GetOperatingSystem() const
    {
        return m_operating_system;
    }

    RenderSystem* Application::GetRenderSystem() const
    {
        return m_render_system;
    }

    PhysicsSystem* Application::GetPhysicsSystem() const
    {
        return m_physics_system;
    }

    LogicSystem* Application::GetLogicSystem() const
    {
        return m_logic_system;
    }

    AnimationSystem* Application::GetAnimationSystem() const
    {
        return m_animation_system;
    }

    FrameUtility* Application::GetFrameUtility() const
    {
        return m_frame_utility;
    }

    TimeUtility* Application::GetApplicationTimer() const
    {
        return m_time_utility;
    }

    LevelManager* Application::GetLevelManager() const
    {
        return m_level_manager;
    }

    SpaceManager* Application::GetSpaceManager() const
    {
        return m_space_manager;
    }

    ObjectFactory* Application::GetObjectFactory() const
    {
        return m_object_factory;
    }

    ComponentRegistry* Application::GetComponentRegistry() const
    {
        return m_component_registry;
    }

    InputCommon* Application::GetInput() const
    {
        return m_input;
    }

    FileUtility* Application::GetFileUtility() const
    {
        return m_file_utility;
    }

    ResourceManager* Application::GetResourceManager() const
    {
        return m_resource_manager;
    }

    GUISystem* Application::GetGUISystem() const
    {
        return m_gui_system;
    }

    GameEditor* Application::GetGameEditor() const
    {
        return m_game_editor;
    }

    void Application::OnResize(int client_width, int client_height) const
    {
        if (m_render_system != nullptr)
        {
            m_render_system->OnResize(client_width, client_height);
        }
    }

    void Application::OnFullscreen() const
    {
        if (m_render_system != nullptr)
        {
            m_render_system->OnFullscreen();
        }
    }

    void Application::OnQuit()
    {
    }

    void Application::LoadSetting()
    {
        std::vector<JsonResource*> resources;
        m_resource_manager->GetJsonResources(eJsonType::Setting, resources);
        if (resources.empty())
        {
            m_initial_setting.b_confine_cursor = false;
            m_initial_setting.b_show_cursor    = true;
            m_initial_setting.b_v_sync         = true;
            m_initial_setting.caption          = "Engine 5th";
            m_initial_setting.screen_height    = 720;
            m_initial_setting.screen_width     = 1280;
            m_initial_setting.screen_scale     = 1.0f;
            m_initial_setting.window_mode      = eWindowMode::Windowed;
            m_initial_setting.initial_level    = "";
        }
        else
        {
            for (auto& resource : resources)
            {
                resource->LoadSetting(m_initial_setting);
            }
        }
    }
}
