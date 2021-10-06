#include "AnimationSubsystem.hpp"

#include "Skeleton/Skeleton.hpp"

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
        //rendering animation related info
        for (auto& skeleton : m_skeletons)
        {
            skeleton->Draw(m_primitive_renderer);
        }
    }

    void AnimationSubsystem::Shutdown()
    {
        //clear skeletons
        {
            for (auto& skeleton : m_skeletons)
            {
                skeleton->Shutdown();
                delete skeleton;
                skeleton = nullptr;
            }
            m_skeletons.clear();
        }
    }

    void AnimationSubsystem::AddSkeleton(Skeleton* skeleton)
    {
        auto found = std::find(m_skeletons.begin(), m_skeletons.end(), skeleton);
        if (found == m_skeletons.end())
        {
            m_skeletons.push_back(skeleton);
        }
    }

    void AnimationSubsystem::RemoveSkeleton(Skeleton* skeleton)
    {
        if (!m_skeletons.empty())
        {
            auto found = std::find(m_skeletons.begin(), m_skeletons.end(), skeleton);
            m_skeletons.erase(found);
        }
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
