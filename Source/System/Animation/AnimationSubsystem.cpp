#include "AnimationSubsystem.hpp"

namespace CS460
{
    AnimationSubsystem::AnimationSubsystem()
    {
    }

    AnimationSubsystem::~AnimationSubsystem()
    {
    }

    void AnimationSubsystem::Initialize()
    {
    }

    void AnimationSubsystem::Update(Real dt)
    {
    }

    void AnimationSubsystem::Render() const
    {
    }

    void AnimationSubsystem::Shutdown()
    {
    }

    void AnimationSubsystem::SetAppUtility(TimeUtility* time_util, FrameUtility* frame_util)
    {
        m_time_utility  = time_util;
        m_frame_utility = frame_util;
    }

    void AnimationSubsystem::SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer)
    {
        m_primitive_renderer = primitive_renderer;
    }
}
