#include "AnimationSubsystem.hpp"

#include "Skeleton/Skeleton.hpp"
#include "Space/AnimationSpace.hpp"

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
        if (m_animation_space == nullptr)
        {
            m_animation_space = new AnimationSpace();
            m_animation_space->Initialize();
        }
    }

    void AnimationSubsystem::Update(Real dt)
    {
        m_animation_space->Update(dt);
        for (auto& skeleton : m_skeletons)
        {
            skeleton->Update(dt);
        }
    }

    void AnimationSubsystem::Render() const
    {
        //rendering animation related info
        m_animation_space->Draw(m_primitive_renderer);
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

        if (m_animation_space != nullptr)
        {
            m_animation_space->Shutdown();
            delete m_animation_space;
            m_animation_space = nullptr;
        }
    }

    void AnimationSubsystem::AddSkeleton(Skeleton* skeleton)
    {
        auto found = std::find(m_skeletons.begin(), m_skeletons.end(), skeleton);
        if (found == m_skeletons.end())
        {
            skeleton->SetSpace(m_animation_space);
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

    void AnimationSubsystem::SetPickingRay(const Ray& ray)
    {
        m_picking_ray = ray;
    }

    bool AnimationSubsystem::Pick(const Ray& ray, Real& min_t, Real& max_t) const
    {
        return m_animation_space->PickBox(ray, min_t, max_t);
    }
}
