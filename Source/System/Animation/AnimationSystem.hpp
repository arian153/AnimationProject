#pragma once
#include <vector>

namespace CS460
{
    class AnimationSubsystem;

    class AnimationSystem
    {
    public:
        AnimationSystem();
        ~AnimationSystem();

        void                Initialize();
        void                Shutdown();
        AnimationSubsystem* CreateSubsystem();
        void                RemoveSubsystem(AnimationSubsystem* subsystem);

    private:
        std::vector<AnimationSubsystem*> m_subsystems;
    };
}
