#pragma once
#include "BroadPhase.hpp"
#include "BoundingAABB.hpp"

namespace CS460
{
    class OctreeNode
    {
    public:
        OctreeNode();
        ~OctreeNode();

        bool        IsLeaf() const;
        OctreeNode* GetChild(const Vector3& pos) const;
        void        SetChild(OctreeNode* child, size_t index);
        Vector3     ScaleSector(size_t index) const;
        bool        Contain(const Vector3& pos) const;

    public:
        size_t                     depth = 0;
        Vector3                    origin;
        Vector3                    scale;
        OctreeNode*                parent = nullptr;
        OctreeNode*                children[8];
        std::vector<BoundingAABB*> aabb_list;
    };

    class Octree final : public BroadPhase
    {
    public:
        Octree();
        ~Octree();

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

        void SetScale(Real x, Real y, Real z);

    private:
        void UpdateNode(OctreeNode* node);
        void ShutdownNode(OctreeNode* node);
        void ReleaseNode(OctreeNode* node);
        void ClearNode(OctreeNode* node);
        void CopyNode(OctreeNode* node, BroadPhase* other) const;
        void IntersectNode(OctreeNode* node, std::list<ColliderPair>& result);
        void CreateChildrenNode(OctreeNode* node) const;

        void InsertAABB(OctreeNode* node, BoundingAABB* aabb) const;
        void IntersectAABB(BoundingAABB* aabb, OctreeNode* node, size_t index, std::list<ColliderPair>& result);
        bool RemoveAABB(OctreeNode* node, BoundingAABB* aabb);

        
    private:
        OctreeNode*                m_root = nullptr;
        std::vector<BoundingAABB*> m_invalid_aabbs;
        size_t                     m_max_depth = 5;
        Vector3                    m_root_scale;
        size_t                     m_max_aabb_count = 5;
    };
}
