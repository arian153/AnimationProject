#pragma once
#include <vector>

#include "../Math/Utility/MathDef.hpp"

namespace CS460
{
    class Skeleton;
    class PrimitiveRenderer;
    class TimeUtility;
    class FrameUtility;

    class AnimationSubsystem
    {
    public:
        AnimationSubsystem();
        ~AnimationSubsystem();

        void Initialize();
        void Update(Real dt);
        void Render() const;
        void Shutdown();

        void AddSkeleton(Skeleton* skeleton);
        void RemoveSkeleton(Skeleton* skeleton);

        void SetAppUtility(TimeUtility* time_util, FrameUtility* frame_util);
        void SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer);

    private:
        TimeUtility*       m_time_utility       = nullptr;
        FrameUtility*      m_frame_utility      = nullptr;
        PrimitiveRenderer* m_primitive_renderer = nullptr;

        std::vector<Skeleton*> m_skeletons;
    };
}
