#pragma once
#include "../ColliderPrimitive.hpp"

namespace CS460
{
    class ColliderPlane
    {
    public:
        ColliderPlane();
        ~ColliderPlane();
        ColliderPlane(const ColliderPlane& rhs)            = delete;
        ColliderPlane& operator=(const ColliderPlane& rhs) = delete;

        void Initialize();
        void Shutdown();

        //minkowski support - gjk, epa
        Vector3 Support(const Vector3& direction);

        //ray - collider intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const;
        Vector3 GetNormal(const Vector3& local_point_on_collider) const;

        //physics
        void SetMassData(Real density);
        Real GetVolume();

        //collider 
        void UpdateBoundingVolume() const;
        void Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const;

    private:
        Vector3 m_normal;
        BoundingAABB* m_bounding_volume = nullptr;
    };
}
