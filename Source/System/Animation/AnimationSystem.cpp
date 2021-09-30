#include "AnimationSystem.hpp"
#include "AnimationSubsystem.hpp"

namespace CS460
{
    AnimationSystem::AnimationSystem()
    {
    }

    AnimationSystem::~AnimationSystem()
    {
    }

    void AnimationSystem::Initialize()
    {
    }

    void AnimationSystem::Shutdown()
    {
        for (auto& subsystem : m_subsystems)
        {
            subsystem->Shutdown();
            delete subsystem;
            subsystem = nullptr;
        }
        m_subsystems.clear();
    }

    AnimationSubsystem* AnimationSystem::CreateSubsystem()
    {
        AnimationSubsystem* subsystem = new AnimationSubsystem();
        subsystem->SetAppUtility(m_time_utility, m_frame_utility);


        m_subsystems.push_back(subsystem);
        return subsystem;
    }

    void AnimationSystem::RemoveSubsystem(AnimationSubsystem* subsystem)
    {
        if (subsystem != nullptr)
        {
            auto found = std::find(m_subsystems.begin(), m_subsystems.end(), subsystem);
            m_subsystems.erase(found);
            subsystem->Shutdown();
            delete subsystem;
            subsystem = nullptr;
        }
    }

    void AnimationSystem::SetAppUtility(TimeUtility* time_util, FrameUtility* frame_util)
    {
        m_time_utility  = time_util;
        m_frame_utility = frame_util;
    }
}
