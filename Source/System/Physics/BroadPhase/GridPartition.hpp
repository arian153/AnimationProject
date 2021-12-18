#pragma once
#include "BroadPhase.hpp"
#include "BoundingAABB.hpp"
#include "../../Core/Utility/CoreDef.hpp"
#include "PotentialPair.hpp"

namespace CS460
{
    class GridCell;

    class GridData
    {
    public:
        GridData(BoundingAABB* bounding_aabb, GridCell* owner_cell, size_t a = Core::U64_MAX, size_t b = Core::U64_MAX);
        ~GridData();

    public:
        BoundingAABB* aabb_data     = nullptr;
        GridCell*     owner_cell    = nullptr;
        size_t        cell_index    = Core::U64_MAX;
        size_t        basis_a_index = Core::U64_MAX;
        size_t        basis_b_index = Core::U64_MAX;
    };

    class GridCell
    {
    public:
        explicit GridCell(size_t index = Core::U64_MAX, size_t a = Core::U64_MAX, size_t b = Core::U64_MAX);
        ~GridCell();

    public:
        size_t                     grid_index;
        size_t                     basis_a_index;
        size_t                     basis_b_index;
        BoundingAABB               cell_boundary;
        std::vector<GridData> data_list;
    };

    class GridPartition final : public BroadPhase
    {
    public:
        GridPartition();
        ~GridPartition();

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        void Copy(BroadPhase* other) override;
        void Add(BoundingAABB* aabb) override;
        void Remove(BoundingAABB* aabb) override;
        void Clear() override;
        void Release() override;
        void Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color) override;
        void ComputePairs(PotentialPairs& potential_pairs) override;

        //Query
        ColliderPrimitive* Pick(const Vector3& point) const override;
        void               Query(const BoundingAABB& aabb, std::vector<ColliderPrimitive*>& output) const override;
        void               CastRay(RayCastResult& result, Real max_distance = -1.0f) const override;
        void               IntersectRay(RayIntersectionResult& result, Real max_distance = -1.0f) const override;
        void               TraceRay(RayTraceResult& result, Real max_distance = -1.0f, size_t reflect_count = 1) const override;

    private:
        class GridDirection
        {
        public:
            int  cell;
            int  delta_cell;
            Real delta_t;
            Real delta_dir_t;
        };

    private:
        void      IntersectAABB(BoundingAABB* aabb, GridCell* cell, size_t index, PotentialPairs& potential_pairs);
        GridCell* QueryCell(const Vector3& point, size_t& a_index, size_t& b_index);
        GridCell* QueryCell(size_t a, size_t b);

        const GridCell* QueryCell(const Vector3& point, size_t& a_index, size_t& b_index) const;
        const GridCell* QueryCell(size_t a, size_t b) const;

        void    PointToIndex(const Vector3& point, size_t& a_index, size_t& b_index) const;
        Vector3 IndexToPoint(size_t a, size_t b) const;

        GridDirection DirectionHelper(Real pos, Real dir) const;
        void          CastRayCell(const GridCell* cell, RayCastResult& result, Real max_distance) const;
        void          IntersectRayCell(const GridCell* cell, RayIntersectionResult& result, Real max_distance) const;
    private:

        size_t  m_cell_scale  = 50;
        size_t  m_extent_a = 400;
        size_t  m_extent_b = 400;
        size_t  m_a_count       = 0;
        size_t  m_b_count       = 0;
        size_t  m_axis_normal   = 1;
        size_t  m_basis_a       = 2;
        size_t  m_basis_b       = 0;
        Vector3 m_center_of_grid;

        std::vector<GridCell>      m_grid_cell_list;
        std::vector<BoundingAABB*> m_invalid_nodes;
    };
}
