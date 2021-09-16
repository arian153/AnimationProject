#pragma once
#include "BroadPhase.hpp"
#include "BoundingAABB.hpp"

namespace CS460
{
    class NSquared final : public BroadPhase
    {
    public:
        NSquared();
        ~NSquared();

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        void Copy(BroadPhase* other) override;
        void Add(BoundingAABB* aabb) override;
        void Remove(BoundingAABB* aabb) override;
        void Clear() override;
        void Release() override;
        void Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color) override;
        void ComputePairs(std::list<ColliderPair>& result) override;

        //Query
        ColliderPrimitive* Pick(const Vector3& point) const override;
        void               Query(const BoundingAABB& aabb, std::vector<ColliderPrimitive*>& output) const override;
        void               CastRay(RayCastResult& result, Real max_distance = -1.0f) const override;
        void               IntersectRay(RayIntersectionResult& result, Real max_distance = -1.0f) const override;
        void               TraceRay(RayTraceResult& result, Real max_distance = -1.0f, size_t reflect_count = 1) const override;

    private:
        std::list<BoundingAABB*> m_aabb_list;
    };
}
