#include "LevelManager.hpp"
#include "Level.hpp"
#include "../../System/Core/Utility/CoreUtility.hpp"
#include "../Resource/ResourceType/JsonResource.hpp"
#include "../Space/SpaceManager.hpp"
#include "../Space/Space.hpp"
#include "../../System/Graphics/RenderSystem.hpp"
#include "../../System/Core/OS-API/Application.hpp"
#include "../../System/Core/Utility/TimeUtility.hpp"
#include "../../System/Core/OS-API/OSCommon.hpp"
#include "../../System/Core/Utility/FrameUtility.hpp"
#include "../../System/Core/Input/InputCommon.hpp"
#include "../Resource/ResourceManager.hpp"
#include "../../System/Graphics/Utility/TextRenderer.hpp"
#include "../../System/GUI/GUISystem.hpp"
#include "../../System/GUI/Editor/GameEditor.hpp"

namespace CS460
{
    LevelManager::LevelManager()
    {
    }

    LevelManager::~LevelManager()
    {
    }

    void LevelManager::Initialize(Application* application)
    {
        m_application       = application;
        m_space_manager     = application->GetSpaceManager();
        m_render_system     = application->GetRenderSystem();
        m_operating_system  = application->GetOperatingSystem();
        m_application_timer = application->GetApplicationTimer();
        m_frame_utility     = application->GetFrameUtility();
        m_input             = application->GetInput();
        m_resource_manager  = application->GetResourceManager();
        m_gui_system        = application->GetGUISystem();
        for (auto& json : m_resource_manager->m_json_resource_map)
        {
            auto level = json.second;
            if (level->IsLevel())
            {
                AddLevel(level);
            }
        }
    }

    void LevelManager::Update()
    {
        //early quit
        if (m_b_set_initial_level == false)
        {
            while (m_b_quit_state_machine == false && m_b_restart == false && m_b_reload == false && m_current == m_next)
            {
                m_application_timer->Tick();
                m_operating_system->DispatchMessagePump();
                m_input->Update();
                if (m_operating_system->IsPaused() == false)
                {
                    m_frame_utility->CalculateFrameStatus(m_application_timer->TotalTime());
                }
                Real time_step = m_application_timer->DeltaTime();

                m_gui_system->Update(time_step);

                m_gui_system->BeginRender();
                m_render_system->BeginUpdate();
                m_gui_system->EndRender();
                m_render_system->EndUpdate();
            }

            m_operating_system->SetQuit(m_b_quit_state_machine);
            return;
        }
        //load phase
        if (m_b_restart == false && m_b_resume == false || m_b_reload == true)
        {
            m_b_reload = false;
            m_current  = m_next;
            if (m_level != nullptr)
            {
                delete m_level;
                m_level = nullptr;
            }
            m_level = CreateLevel(m_current);
            LoadLevel(m_level);
        }
        else
        {
            m_b_restart = false;
        }
        //init phase
        if (m_b_resume == true)
        {
            m_b_resume = false;
            delete m_level;
            m_level = m_pause_levels.front().level;
            m_pause_levels.pop_front();
            m_current = m_next;
            //only resume and restart case
            if (m_b_resume_restart == true)
            {
                m_b_resume_restart = false;
                ShutdownLevel(m_level);
                InitializeLevel(m_level);
            }
        }
        else
        {
            InitializeLevel(m_level);
        }
        //update phase
        while (m_b_quit_state_machine == false && m_b_restart == false && m_b_reload == false && m_current == m_next)
        {
            m_application_timer->Tick();
            m_operating_system->DispatchMessagePump();
            m_input->Update();
            if (m_operating_system->IsPaused() == false)
            {
                m_frame_utility->CalculateFrameStatus(m_application_timer->TotalTime());
            }
            Real time_step = m_application_timer->DeltaTime();
            UpdateLevel(m_level, time_step);
            m_elapsed_time += time_step;
            if (m_elapsed_time >= m_fixed_time_step)
            {
                //Console::ConsoleOut(time_step, m_elapsed_time, m_frame_utility->GetFramePerSecond(), "\n");
                FixedUpdateLevel(m_level, m_fixed_time_step);
                m_elapsed_time   = 0.0f;
                m_b_fixed_update = true;
            }
            RenderLevel(m_level, time_step);
        }
        //shutdown phase
        if (m_b_pause_and_change == false)
        {
            ShutdownLevel(m_level);
            //unload phase
            if (m_b_restart == false || m_b_resume_restart == true || m_b_reload == true)
            {
                UnloadLevel(m_level);
            }
        }
        else
        {
            m_b_pause_and_change = false;
            m_level              = nullptr;
        }
        m_operating_system->SetQuit(m_b_quit_state_machine);
    }

    void LevelManager::Shutdown()
    {
        delete m_level;
        m_level = nullptr;
        RemovePausedLevel();
        m_level_resources.clear();
    }

    void LevelManager::AddLevel(const std::string& level_name)
    {
        auto found = m_level_resources.find(level_name);
        if (found == m_level_resources.end())
        {
            std::wstring  level_path      = m_resource_manager->GetRootPath() + L"/Data/Level/" + ToWString(level_name) + L".json";
            std::wstring  space_path      = L"Data/Space/" + ToWString(level_name) + L"World.json";
            JsonResource* level_resource  = m_resource_manager->CreateJsonResource(level_path);
            level_resource->m_b_load_type = false;
            level_resource->Initialize();
            level_resource->m_json_type = eJsonType::Level;
            level_resource->CreateLevelData(space_path);
            level_resource->m_b_load_type = true;
            m_level_resources.emplace(level_name, level_resource);
            JsonResource* space_resource  = m_resource_manager->CreateJsonResource(m_resource_manager->GetRootPath() + L"/" + space_path);
            space_resource->m_b_load_type = false;
            space_resource->Initialize();
            space_resource->m_json_type = eJsonType::Space;
            space_resource->CreateSpaceData();
            space_resource->m_b_load_type = true;
        }
    }

    void LevelManager::AddLevel(JsonResource* level_resource)
    {
        std::string level_name = ToString(level_resource->WFileName());
        auto        found      = m_level_resources.find(level_name);
        if (found == m_level_resources.end())
        {
            m_level_resources.emplace(level_name, level_resource);
        }
    }

    void LevelManager::SetInitialLevel(const std::string& level_name)
    {
        if (level_name == "")
        {
            return;
        }

        m_b_set_initial_level = true;
        m_current             = level_name;
        m_next                = level_name;
    }

    void LevelManager::EnableLoadPhase(bool b_enable_load_phase)
    {
        m_b_enable_load_phase = b_enable_load_phase;
    }

    void LevelManager::SetQuit()
    {
        m_b_quit_state_machine = true;
    }

    void LevelManager::ChangeLevel(const std::string& level_name)
    {
        m_next = level_name;
        RemovePausedLevel();
    }

    void LevelManager::RestartLevel()
    {
        m_b_restart = true;
    }

    void LevelManager::ReloadLevel()
    {
        m_b_reload = true;
    }

    void LevelManager::PauseAndChangeLevel(const std::string& level_name)
    {
        m_b_pause_and_change = true;
        PauseInfo info;
        info.type  = m_current;
        info.level = m_level;
        m_pause_levels.push_front(info);
        m_next = level_name;
    }

    bool LevelManager::ResumeLevel()
    {
        if (m_pause_levels.empty() == false)
        {
            m_b_resume = true;
            m_next     = m_pause_levels.front().type;
            return true;
        }
        return false;
    }

    bool LevelManager::ResumeAndRestartLevel()
    {
        if (m_pause_levels.empty() == false)
        {
            m_next             = m_pause_levels.front().type;
            m_b_restart        = true;
            m_b_resume         = true;
            m_b_resume_restart = true;
            return true;
        }
        return false;
    }

    std::string LevelManager::GetCurrentLevelName() const
    {
        return m_current;
    }

    Level* LevelManager::GetCurrentLevel() const
    {
        return m_level;
    }

    void LevelManager::UpdatePausedSpace(Real dt, const std::string& level_name, size_t space_index, eSubsystemFlag flag)
    {
        if (m_pause_levels.empty() == false)
        {
            for (auto info : m_pause_levels)
            {
                if (info.type == level_name)
                {
                    info.level->UpdateSpace(dt, space_index, flag);
                }
            }
        }
    }

    void LevelManager::UpdatePausedLevels(Real dt)
    {
        if (m_pause_levels.empty() == false)
        {
            for (auto info : m_pause_levels)
            {
                UpdateLevel(info.level, dt);
            }
        }
    }

    void LevelManager::RemovePausedLevel()
    {
        while (m_pause_levels.empty() == false)
        {
            auto info = m_pause_levels.front();
            ShutdownLevel(info.level);
            UnloadLevel(info.level);
            delete info.level;
            m_pause_levels.pop_front();
        }
    }

    void LevelManager::InitializeLevel(Level* level) const
    {
        level->m_global_space = m_space_manager->GetGlobalSpace();
        for (auto& resource : level->m_space_resources)
        {
            auto space = m_space_manager->CreateSpace(level, resource);
            level->m_spaces.push_back(space);
        }
        level->Initialize();
    }

    void LevelManager::UpdateLevel(Level* level, Real dt) const
    {
        level->UpdateSubsystem(dt, eSubsystemFlag::Logic);
        level->UpdateSubsystem(dt, eSubsystemFlag::World);
        level->UpdateSubsystem(dt, eSubsystemFlag::Animation);
        //update sound
        level->UpdateSubsystem(dt, eSubsystemFlag::Scene);
        level->Update(dt);
        m_gui_system->Update(dt);
    }

    void LevelManager::FixedUpdateLevel(Level* level, Real dt) const
    {
        level->FixedUpdateSubsystem(dt, eSubsystemFlag::Logic);
        level->FixedUpdateSubsystem(dt, eSubsystemFlag::World);
        level->FixedUpdateSubsystem(dt, eSubsystemFlag::Animation);
        //update sound
        level->FixedUpdateSubsystem(dt, eSubsystemFlag::Scene);
        level->FixedUpdate(dt);
    }

    void LevelManager::RenderLevel(Level* level, Real dt) const
    {
        level->DrawSubsystem(eSubsystemFlag::Logic);
        level->DrawSubsystem(eSubsystemFlag::World);
        level->DrawSubsystem(eSubsystemFlag::Animation);
        //render scene
        m_gui_system->BeginRender();
        m_render_system->BeginUpdate();
        level->DrawSubsystem(eSubsystemFlag::Scene);
        m_gui_system->EndRender();
        m_render_system->EndUpdate();
    }

    void LevelManager::ShutdownLevel(Level* level) const
    {
        level->Shutdown();
        for (auto& space : level->m_spaces)
        {
            m_space_manager->RemoveSpace(space);
        }
        level->m_space_resources.clear();
    }

    void LevelManager::LoadLevel(Level* level) const
    {
        if (m_b_enable_load_phase)
        {
            level->Load();
        }
    }

    void LevelManager::UnloadLevel(Level* level) const
    {
        if (m_b_enable_load_phase)
        {
            level->Unload();
        }
    }

    Level* LevelManager::CreateLevel(const std::string& level_name)
    {
        Level* created = nullptr;
        auto   found   = m_level_resources.find(level_name);
        if (found != m_level_resources.end())
        {
            created       = new Level(level_name);
            auto resource = found->second;
            if (resource->IsLevel())
            {
                resource->LoadData(created);
            }
        }
        return created;
    }

    void LevelManager::UpdateActive()
    {
    }

    void LevelManager::UpdatePaused()
    {
    }
}
