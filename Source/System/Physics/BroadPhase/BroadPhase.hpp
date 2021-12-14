#pragma once
#include "RayCast.hpp"
#include <list>
#include <vector>
#include "ColliderPair.hpp"

namespace CS460
{
    class ColorFlag;
    class ColliderPrimitive;
    class BoundingAABB;
    class PrimitiveRenderer;
    class Color;

    class BroadPhase
    {
    public:
        virtual      ~BroadPhase() = default;
        virtual void Initialize() = 0;
        virtual void Update(Real dt) = 0;
        virtual void Shutdown() = 0;
        virtual void Copy(BroadPhase* other) = 0;
        virtual void Add(BoundingAABB* aabb) = 0;
        virtual void Remove(BoundingAABB* aabb) = 0;
        virtual void Clear() = 0;
        virtual void Release() = 0;
        virtual void Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color) = 0;
        virtual void ComputePairs(std::list<PotentialPair>& result) = 0;

        //Query
        virtual ColliderPrimitive* Pick(const Vector3& point) const = 0;
        virtual void               Query(const BoundingAABB& aabb, std::vector<ColliderPrimitive*>& output) const = 0;
        virtual void               CastRay(RayCastResult& result, Real max_distance = -1.0f) const = 0;
        virtual void               IntersectRay(RayIntersectionResult& result, Real max_distance = -1.0f) const = 0;
        virtual void               TraceRay(RayTraceResult& result, Real max_distance = -1.0f, size_t reflect_count = 1) const = 0;

    public:
    };
}
