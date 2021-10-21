#pragma once
#include <vector>


#include "../Math/Primitive/Others/Ray.hpp"
#include "../Math/Utility/MathDef.hpp"

namespace CS460
{
    class Ray;
    class AnimationSpace;
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
        void SetPickingRay(const Ray& ray);

        bool Pick(const Ray& ray, Real& min_t, Real& max_t) const;

    private:
        TimeUtility*       m_time_utility       = nullptr;
        FrameUtility*      m_frame_utility      = nullptr;
        PrimitiveRenderer* m_primitive_renderer = nullptr;
        AnimationSpace*    m_animation_space    = nullptr;
        Ray                m_picking_ray;

        std::vector<Skeleton*> m_skeletons;
    };
}
