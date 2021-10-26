#pragma once
#include "SpacePath.hpp"
#include "../../Math/Primitive/ConvexHull3D/Box.hpp"
#include "../../Math/Primitive/ConvexHull3D/Sphere.hpp"
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

        bool PickBox(const Ray& picking_ray, Real& min_t, Real& max_t);

        //E5_TODO - add broad phase for performance later.
    private:
        //this can be changed terrain structure.
        Box          m_platform_box;
        VQSTransform m_box_transform;

        SpacePath m_test_path;
        Sphere    m_drawing_sphere;
        Sphere    m_moving_sphere; //temp
        Real      m_test_s = 0.0f;
        Real      m_test_speed = 10.0f;

        std::vector<Vector3> control_points;
    };
}
