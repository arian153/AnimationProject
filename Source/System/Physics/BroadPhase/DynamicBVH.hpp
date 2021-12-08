#pragma once
#include "BroadPhase.hpp"
#include "BoundingAABB.hpp"
#include "../../Math/Primitive/ConvexHull3D/Box.hpp"

namespace CS460
{
    class DynamicBVHNode
    {
    public:
        DynamicBVHNode();
        ~DynamicBVHNode();
        bool            IsLeaf() const;
        void            SetBranch(DynamicBVHNode* n0, DynamicBVHNode* n1);
        void            SetLeaf(BoundingAABB* bounding_aabb);
        void            UpdateAABB(Real margin);
        DynamicBVHNode* GetSibling() const;

    public:
        DynamicBVHNode* parent = nullptr;
        DynamicBVHNode* children[2];
        bool            children_crossed = false;
        BoundingAABB    aabb;
        BoundingAABB*   data = nullptr;
    };

    class DynamicBVH final : public BroadPhase
    {
    public:
        DynamicBVH();
        ~DynamicBVH();

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
        void UpdateNodeRecursive(DynamicBVHNode* node, std::vector<DynamicBVHNode*>& invalid_nodes);
        void InsertNodeRecursive(DynamicBVHNode* node, DynamicBVHNode** parent) const;
        void RemoveNodeRecursive(DynamicBVHNode* node);
        void ClearNodeRecursive(DynamicBVHNode* node);
        void ReleaseNodeRecursive(DynamicBVHNode* node);
        void ShutdownNodeRecursive(DynamicBVHNode* node);
        void CopyNodeRecursive(DynamicBVHNode* node, BroadPhase* other);
        void RenderNodeRecursive(DynamicBVHNode* node, PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color);
        void ComputePairsRecursive(DynamicBVHNode* n0, DynamicBVHNode* n1, std::list<ColliderPair>& result);
        void ClearChildrenCrossFlagRecursive(DynamicBVHNode* node);
        void CrossChildren(DynamicBVHNode* node, std::list<ColliderPair>& result);

    private:
        DynamicBVHNode*              m_root = nullptr;
        std::vector<DynamicBVHNode*> m_invalid_nodes;
        Box                          m_drawing_box;
    };
}
