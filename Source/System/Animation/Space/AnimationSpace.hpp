#pragma once
#include "../../Math/Primitive/ConvexHull3D/Box.hpp"
#include "../../Math/Primitive/Others/Plane.hpp"
#include "../../Math/Structure/Transform.hpp"
#include "../../Math/Structure/VQSTransform.hpp"

namespace CS460
{
    class AnimationSpace
    {
    public:
        AnimationSpace();
        ~AnimationSpace();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        void Draw(PrimitiveRenderer* renderer) const;

        bool PickBox(const Ray& picking_ray, Real& min_t, Real& max_t) ;

        //E5_TODO - add broad phase for performance later.
    private:
        //this can be changed terrain structure.
        Box          m_platform_box;
        VQSTransform m_box_transform;

        std::vector<Vector3> control_points;
    };
}
