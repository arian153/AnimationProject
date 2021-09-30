#pragma once
#include <vector>

namespace CS460
{
    class FrameUtility;
    class TimeUtility;
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

        void SetAppUtility(TimeUtility* time_util, FrameUtility* frame_util);

    private:
        TimeUtility*  m_time_utility  = nullptr;
        FrameUtility* m_frame_utility = nullptr;

        std::vector<AnimationSubsystem*> m_subsystems;
    };
}
