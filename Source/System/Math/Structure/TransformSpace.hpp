#pragma once
#include <vector>

#include "../Primitive/ConvexHull3D/Box.hpp"

namespace CS460
{
    class RayCastResult;
    class Transform;

    class RayCastData
    {
    public:
        bool         operator<(const RayCastData& rhs) const;
        RayCastData& operator=(const RayCastData& rhs);

    public:
        Ray        ray;
        Transform* transform = nullptr;
        bool       hit       = false;
        Real       t         = -1.0f;
        Vector3    intersection;
    };

    class TransformSpace
    {
    public:
        TransformSpace();
        ~TransformSpace();

        void Initialize();
        void Update(Real dt);
        void Shutdown();
        void Render(PrimitiveRenderer* primitive_renderer, const Color& color);

        void Add(Transform* transform);
        void Remove(Transform* transform);
       
        void CastRay(RayCastData& result, Real max_distance = -1.0f) const;

    private:
        std::vector<Transform*> m_transforms;
        Box                     m_drawing_box;
    };
}
