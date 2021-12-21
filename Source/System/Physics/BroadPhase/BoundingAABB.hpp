#pragma once
#include "..//..//Math/Math.hpp"

namespace CS460
{
    class SoftBody;
    class ColliderSet;
    class ColliderPrimitive;

    enum class eBPDType
    {
        Collider,
        SoftBody,
        Invalid
    };

    class BoundingAABB
    {
    public:
        explicit BoundingAABB(ColliderPrimitive* collider = nullptr);
        explicit BoundingAABB(SoftBody* softbody);
        ~BoundingAABB();

        void         Set(const Vector3& min, const Vector3& max);
        void         ExpandMargin(Real margin);
        bool         Intersect(BoundingAABB* aabb) const;
        bool         Intersect(const BoundingAABB& aabb) const;
        bool         Intersect(const Vector3& pos, Real radius) const;
        bool         Contains(const Vector3& point) const;
        bool         TestRayIntersection(const Ray& ray, Real& t, Real max_distance = -1.0f) const;
        bool         Contains(BoundingAABB* aabb) const;
        Real         Volume() const;
        Vector3      Center() const;
        Vector3      Size() const;
        Vector3      HalfSize() const;
        Vector3      Min() const;
        Vector3      Max() const;
        BoundingAABB Union(const BoundingAABB& aabb) const;

        //getter
        ColliderPrimitive* GetCollider() const;
        ColliderSet*       GetColliderSet() const;
        SoftBody*          GetSoftBody() const;
        void*              GetUserData() const;
    private:
        friend class DynamicBVHNode;
        friend class DynamicBVH;
        friend class GridData;
        friend class GridPartition;
    private:
        Vector3            m_min;
        Vector3            m_max;
        eBPDType           m_bpd_type     = eBPDType::Invalid;
        ColliderSet*       m_collider_set = nullptr;
        ColliderPrimitive* m_collider     = nullptr;
        SoftBody*          m_softbody     = nullptr;
        void*              m_userdata     = nullptr;
    };
}
