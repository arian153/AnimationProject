#include "GridPartition.hpp"
#include "..//Dynamics/RigidBody.hpp"
#include <algorithm>

#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace CS460
{
    GridData::GridData(BoundingAABB* bounding_aabb, GridCell* owner_cell, size_t a, size_t b)
        : aabb_data(bounding_aabb), owner_cell(owner_cell), basis_a_index(a), basis_b_index(b)
    {
        bounding_aabb->m_userdata = this;
        cell_index                = owner_cell->data_list.size();
    }

    GridData::~GridData()
    {
    }

    GridCell::GridCell(size_t index, size_t a, size_t b)
        : grid_index(index), basis_a_index(a), basis_b_index(b)
    {
    }

    GridCell::~GridCell()
    {
    }

    GridPartition::GridPartition()
    {
    }

    GridPartition::~GridPartition()
    {
    }

    void GridPartition::Initialize()
    {
        m_a_count    = (size_t)ceil((Real)m_extent_a / m_cell_scale);
        m_b_count    = (size_t)ceil((Real)m_extent_b / m_cell_scale);
        size_t count = m_a_count * m_b_count;
        m_grid_cell_list.resize(count);

        Vector3 cell_start;
        cell_start[m_basis_a] = -(Real)m_extent_a * 0.5f;
        cell_start[m_basis_b] = -(Real)m_extent_b * 0.5f;
        Real size_of_cell     = static_cast<Real>(m_cell_scale);

        for (size_t i = 0; i < count; i++)
        {
            size_t a_index = i % m_b_count;
            size_t b_index = i / m_b_count;

            m_grid_cell_list[i].grid_index    = i;
            m_grid_cell_list[i].basis_a_index = a_index;
            m_grid_cell_list[i].basis_b_index = b_index;
            m_grid_cell_list[i].data_list.reserve(20);
            Vector3 cell_min;
            cell_min[m_basis_a]     = static_cast<Real>(a_index) * size_of_cell;
            cell_min[m_basis_b]     = static_cast<Real>(b_index) * size_of_cell;
            cell_min[m_axis_normal] = -size_of_cell;// Math::REAL_MAX + 1.0f;
            Vector3 cell_max;
            cell_max[m_basis_a]     = static_cast<Real>(a_index + 1) * size_of_cell;
            cell_max[m_basis_b]     = static_cast<Real>(b_index + 1) * size_of_cell;
            cell_max[m_axis_normal] = +size_of_cell;// Math::REAL_MAX - 1.0f;
            m_grid_cell_list[i].cell_boundary.Set(cell_start + cell_min, cell_start + cell_max);
        }
    }

    void GridPartition::Update(Real dt)
    {
        //check validate.
        m_invalid_nodes.clear();
        for (auto& cell : m_grid_cell_list)
        {
            for (auto& grid_data : cell.data_list)
            {
                if (cell.cell_boundary.Contains(grid_data.aabb_data->Center()) == false)
                {
                    //invalid aabb
                    m_invalid_nodes.push_back(grid_data.aabb_data);
                }
            }
        }
        for (auto aabb : m_invalid_nodes)
        {
            Remove(aabb);
            Add(aabb);
        }
    }

    void GridPartition::Shutdown()
    {
        for (auto& cell : m_grid_cell_list)
        {
            for (auto& grid_data : cell.data_list)
            {
                if (grid_data.aabb_data != nullptr)
                {
                    delete grid_data.aabb_data;
                    grid_data.aabb_data = nullptr;
                }
            }
            cell.data_list.clear();
        }
        m_grid_cell_list.clear();
    }

    void GridPartition::Copy(BroadPhase* other)
    {
        for (auto& cell : m_grid_cell_list)
        {
            for (auto& grid_data : cell.data_list)
            {
                other->Add(grid_data.aabb_data);
            }
        }
    }

    void GridPartition::Add(BoundingAABB* aabb)
    {
        size_t    a_index, b_index;
        GridCell* cell = QueryCell(aabb->Center(), a_index, b_index);
        cell->data_list.emplace_back(aabb, cell, a_index, b_index);
    }

    void GridPartition::Remove(BoundingAABB* aabb)
    {
        if (aabb->m_userdata != nullptr)
        {
            GridData*              grid_data = static_cast<GridData*>(aabb->m_userdata);
            std::vector<GridData>& aabb_list = grid_data->owner_cell->data_list;
            aabb_list[grid_data->cell_index] = aabb_list.back();
            aabb_list.pop_back();
            if (grid_data->cell_index < aabb_list.size())
            {
                static_cast<GridData*>(aabb_list[grid_data->cell_index].aabb_data->m_userdata)->cell_index = grid_data->cell_index;
            }
            aabb->m_userdata = nullptr;
        }
    }

    void GridPartition::Clear()
    {
        for (auto& cell : m_grid_cell_list)
        {
            cell.data_list.clear();
        }
        m_grid_cell_list.clear();
    }

    void GridPartition::Release()
    {
        for (auto& cell : m_grid_cell_list)
        {
            for (auto& aabb : cell.data_list)
            {
                if (aabb.aabb_data != nullptr)
                {
                    delete aabb.aabb_data;
                    aabb.aabb_data = nullptr;
                }
            }
        }
    }

    void GridPartition::Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color)
    {
        for (auto& cell : m_grid_cell_list)
        {
            primitive_renderer->DrawBox(
                                        cell.cell_boundary.Center(), Quaternion(),
                                        cell.cell_boundary.Size(), eRenderingMode::Line, broad_phase_color.color);
            for (auto& aabb : cell.data_list)
            {
                primitive_renderer->DrawBox(
                                            aabb.aabb_data->Center(), Quaternion(),
                                            aabb.aabb_data->Size(), eRenderingMode::Line, broad_phase_color.color);

                if (aabb.aabb_data->GetCollider() != nullptr && primitive_color.b_flag)
                {
                    aabb.aabb_data->GetCollider()->Draw(primitive_renderer, eRenderingMode::Line, primitive_color.color);
                }
            }
        }
    }

    void GridPartition::ComputePairs(PotentialPairs& result)
    {
        size_t cell_size = m_grid_cell_list.size();
        for (size_t i = 0; i < cell_size; ++i)
        {
            GridCell& cell = m_grid_cell_list[i];
            // Loop through all balls in a cell
            size_t aabb_size = cell.data_list.size();
            for (size_t j = 0; j < aabb_size; ++j)
            {
                BoundingAABB* aabb = cell.data_list[j].aabb_data;
                // Update with the residing cell
                IntersectAABB(aabb, &cell, j + 1, result);
                GridData* grid_data = static_cast<GridData*>(aabb->m_userdata);
                size_t    a_index   = grid_data->basis_a_index;
                size_t    b_index   = grid_data->basis_b_index;
                // Update collision with neighbor cells
                if (a_index > 0)
                {
                    IntersectAABB(aabb, QueryCell(a_index - 1, b_index), 0, result);
                    if (b_index < m_b_count - 1)
                    {
                        IntersectAABB(aabb, QueryCell(a_index - 1, b_index + 1), 0, result);
                    }
                    if (b_index > 0)
                    {
                        IntersectAABB(aabb, QueryCell(a_index - 1, b_index - 1), 0, result);
                    }
                }
                if (b_index < m_b_count - 1)
                {
                    IntersectAABB(aabb, QueryCell(a_index, b_index + 1), 0, result);
                }
            }
        }
    }

    ColliderPrimitive* GridPartition::Pick(const Vector3& point) const
    {
        size_t a_index, b_index;
        auto   cell = QueryCell(point, a_index, b_index);
        for (auto& aabb : cell->data_list)
        {
            if (aabb.aabb_data->Contains(point) == true)
            {
                return aabb.aabb_data->GetCollider();
            }
        }
        return nullptr;
    }

    void GridPartition::Query(const BoundingAABB& aabb, std::vector<ColliderPrimitive*>& output) const
    {
        size_t min_a, min_b, max_a, max_b;
        QueryCell(aabb.m_min, min_a, min_b);
        QueryCell(aabb.m_max, max_a, max_b);
        for (size_t i = min_a; i <= max_a; ++i)
        {
            for (size_t j = min_b; j <= max_b; ++j)
            {
                auto cell = QueryCell(i, j);
                for (auto& collider_aabb : cell->data_list)
                {
                    if (collider_aabb.aabb_data->Intersect(aabb) == true)
                    {
                        output.push_back(collider_aabb.aabb_data->GetCollider());
                    }
                }
            }
        }
    }

    void GridPartition::CastRay(RayCastResult& result, Real max_distance) const
    {
        Ray     ray          = result.ray;
        Vector3 ab_direction = ray.direction;
        //  auto    dir_a        = DirectionHelper(ray.position[m_basis_a] + m_center_of_grid[m_basis_a], ray.direction[m_basis_a]);
        //auto    dir_b        = DirectionHelper(ray.position[m_basis_b] + m_center_of_grid[m_basis_b], ray.direction[m_basis_b]);
        // Real    t            = 0.0f;
        //remove perpendicular
        ab_direction[m_axis_normal] = 0.0f;
        Vector3 ray_unit_step, ray_length;

        int    step_a, step_b;
        size_t cell_a, cell_b;

        PointToIndex(ray.position, cell_a, cell_b);

        int grid_a = (int)ray.position[m_basis_a];
        int grid_b = (int)ray.position[m_basis_b];

        ray_unit_step[m_basis_a] = sqrtf(1.0f + ((ray.direction[m_basis_b] / ray.direction[m_basis_a]) * (ray.direction[m_basis_b] / ray.direction[m_basis_a])));
        ray_unit_step[m_basis_b] = sqrtf(1.0f + ((ray.direction[m_basis_a] / ray.direction[m_basis_b]) * (ray.direction[m_basis_a] / ray.direction[m_basis_b])));

        if (ray_unit_step[m_basis_a] < 0)
        {
            step_a                = -1;
            ray_length[m_basis_a] = (ray.position[m_basis_a] - (Real)grid_a) * ray_unit_step[m_basis_a];
        }
        else
        {
            step_a                = 1;
            ray_length[m_basis_a] = ((Real)(grid_a + 1) - ray.position[m_basis_a]) * ray_unit_step[m_basis_a];
        }
        if (ray_unit_step[m_basis_b] < 0)
        {
            step_b                = -1;
            ray_length[m_basis_b] = (ray.position[m_basis_b] - (Real)grid_b) * ray_unit_step[m_basis_b];
        }
        else
        {
            step_b                = 1;
            ray_length[m_basis_b] = ((Real)(grid_b + 1) - ray.position[m_basis_b]) * ray_unit_step[m_basis_b];
        }

        if (ab_direction.LengthSquared() > 0.0f)
        {
            float t = 0.0f;

            while ((cell_a > 0 && cell_a <= m_extent_a && cell_b > 0 && cell_b <= m_extent_b))
            {
                CastRayCell(QueryCell(cell_a, cell_b), result, max_distance);
                if (result.hit_data.hit)
                {
                    break;
                }
                if (ray_length[m_basis_a] < ray_length[m_basis_b])
                {
                    grid_a += step_a;
                    cell_a += step_a;
                    t = ray_length[m_basis_a];
                    ray_length[m_basis_a] += ray_unit_step[m_basis_a];
                }
                else
                {
                    grid_b += step_b;
                    cell_b += step_b;
                    t = ray_length[m_basis_b];
                    ray_length[m_basis_b] += ray_unit_step[m_basis_b];
                }
            }
        }
        else
        {
            //ray direction is perpendicular to grid plane
            CastRayCell(QueryCell(grid_a, grid_b), result, max_distance);
        }
    }

    void GridPartition::IntersectRay(RayIntersectionResult& result, Real max_distance) const
    {
    }

    void GridPartition::TraceRay(RayTraceResult& result, Real max_distance, size_t reflect_count) const
    {
    }

    void GridPartition::IntersectAABB(BoundingAABB* aabb, GridCell* cell, size_t index, PotentialPairs& result)
    {
        size_t size = cell->data_list.size();
        for (size_t i = index; i < size; i++)
        {
            ColliderPrimitive* collider_a = aabb->GetCollider();
            ColliderPrimitive* collider_b = cell->data_list[i].aabb_data->GetCollider();
            if (aabb->Intersect(cell->data_list[i].aabb_data))
            {
                result.emplace_back(collider_a, collider_b);
            }
        }
    }

    GridCell* GridPartition::QueryCell(const Vector3& point, size_t& a_index, size_t& b_index)
    {
        //translate point position
        Real   a_pos  = (point[m_basis_a] + m_center_of_grid[m_basis_a]) / (Real)m_cell_scale;
        Real   b_pos  = (point[m_basis_b] + m_center_of_grid[m_basis_b]) / (Real)m_cell_scale;
        size_t cell_a = static_cast<size_t>(a_pos);
        size_t cell_b = static_cast<size_t>(b_pos);
        if (a_pos < 0.0f)
        {
            cell_a = 0;
        }
        else if (cell_a >= m_a_count)
        {
            cell_a = m_a_count - 1;
        }
        if (b_pos < 0.0f)
        {
            cell_b = 0;
        }
        else if (cell_b >= m_b_count)
        {
            cell_b = m_b_count - 1;
        }
        a_index = cell_a;
        b_index = m_a_count * cell_b;
        return &m_grid_cell_list[a_index + b_index];
    }

    GridCell* GridPartition::QueryCell(size_t a, size_t b)
    {
        if (a >= m_a_count)
        {
            a = m_a_count - 1;
        }
        if (b >= m_b_count)
        {
            b = m_b_count - 1;
        }
        size_t a_index = a;
        size_t b_index = m_a_count * b;
        return &m_grid_cell_list[a_index + b_index];
    }

    const GridCell* GridPartition::QueryCell(const Vector3& point, size_t& a_index, size_t& b_index) const
    {
        Real   a_pos  = (point[m_basis_a] + m_center_of_grid[m_basis_a]) / (Real)m_cell_scale;
        Real   b_pos  = (point[m_basis_b] + m_center_of_grid[m_basis_b]) / (Real)m_cell_scale;
        size_t cell_a = static_cast<size_t>(a_pos);
        size_t cell_b = static_cast<size_t>(b_pos);
        if (a_pos < 0.0f)
        {
            cell_a = 0;
        }
        else if (cell_a >= m_a_count)
        {
            cell_a = m_a_count - 1;
        }
        if (b_pos < 0.0f)
        {
            cell_b = 0;
        }
        else if (cell_b >= m_b_count)
        {
            cell_b = m_b_count - 1;
        }
        a_index = cell_a;
        b_index = m_a_count * cell_b;
        return &m_grid_cell_list[a_index + b_index];
    }

    const GridCell* GridPartition::QueryCell(size_t a, size_t b) const
    {
        if (a >= m_a_count)
        {
            a = m_a_count - 1;
        }
        if (b >= m_b_count)
        {
            b = m_b_count - 1;
        }
        size_t a_index = a;
        size_t b_index = m_a_count * b;
        return &m_grid_cell_list[a_index + b_index];
    }

    void GridPartition::PointToIndex(const Vector3& point, size_t& a_index, size_t& b_index) const
    {
        Real   a_pos  = (point[m_basis_a] + m_center_of_grid[m_basis_a]) / (Real)m_cell_scale;
        Real   b_pos  = (point[m_basis_b] + m_center_of_grid[m_basis_b]) / (Real)m_cell_scale;
        size_t cell_a = static_cast<size_t>(a_pos);
        size_t cell_b = static_cast<size_t>(b_pos);
        if (a_pos < 0.0f)
        {
            cell_a = 0;
        }
        else if (cell_a >= m_a_count)
        {
            cell_a = m_a_count - 1;
        }
        if (b_pos < 0.0f)
        {
            cell_b = 0;
        }
        else if (cell_b >= m_b_count)
        {
            cell_b = m_b_count - 1;
        }
        a_index = cell_a;
        b_index = m_a_count * cell_b;
    }

    Vector3 GridPartition::IndexToPoint(size_t a, size_t b) const
    {
        if (a >= m_a_count)
        {
            a = m_a_count - 1;
        }
        if (b >= m_b_count)
        {
            b = m_b_count - 1;
        }
        size_t a_index = a;
        size_t b_index = m_a_count * b;
        return m_grid_cell_list[a_index + b_index].cell_boundary.m_min;
    }

    GridPartition::GridDirection GridPartition::DirectionHelper(Real pos, Real dir) const
    {
        GridDirection result;
        result.cell = static_cast<int>(std::floorf(pos / (Real)m_cell_scale)) + 1;
        if (Math::IsGreatorThanZero(dir) == true)
        {
            result.delta_cell = 1;
            result.delta_t    = (result.cell * m_cell_scale - pos) / dir;
        }
        else
        {
            result.delta_cell = -1;
            result.delta_t    = ((result.cell - 1) * m_cell_scale - pos) / dir;
        }
        result.delta_dir_t = result.delta_cell * m_cell_scale / dir;
        return result;
    }

    void GridPartition::CastRayCell(const GridCell* cell, RayCastResult& result, Real max_distance) const
    {
        Ray ray = result.ray;
        // test AABBs for candidates
        std::vector<ColliderPrimitive*> candidate_list;
        candidate_list.reserve(cell->data_list.size());
        Real t = Math::REAL_POSITIVE_MAX;
        for (auto& aabb : cell->data_list)
        {
            if (aabb.aabb_data->TestRayIntersection(ray, t, max_distance) == true)
            {
                candidate_list.push_back(aabb.aabb_data->GetCollider());
            }
        }
        // test actual colliders
        std::vector<HitData> result_list;
        result_list.reserve(candidate_list.size());
        for (ColliderPrimitive* collider : candidate_list)
        {
            RayCastResult ray_cast_result(ray);
            collider->CastRay(ray_cast_result, max_distance);
            if (ray_cast_result.hit_data.hit == true)
            {
                result_list.push_back(ray_cast_result.hit_data);
            }
        }
        // sort the result list
        std::sort(result_list.begin(), result_list.end());
        if (result_list.empty() == false)
        {
            // the first result entry is the closest one
            HitData& entry  = result_list.front();
            result.hit_data = entry;
        }
        else
        {
            result.hit_data.hit = false;
        }
    }

    void GridPartition::IntersectRayCell(const GridCell* cell, RayIntersectionResult& result, Real max_distance) const
    {
        Ray ray = result.ray;
        // test AABBs for candidates
        std::vector<ColliderPrimitive*> candidate_list;
        candidate_list.reserve(cell->data_list.size());
        Real t = Math::REAL_POSITIVE_MAX;
        for (auto& aabb : cell->data_list)
        {
            if (aabb.aabb_data->TestRayIntersection(ray, t, max_distance) == true)
            {
                candidate_list.push_back(aabb.aabb_data->GetCollider());
            }
        }
        // test actual colliders
        result.hit_list.reserve(candidate_list.size());
        for (ColliderPrimitive* collider : candidate_list)
        {
            RayCastResult ray_cast_result(ray);
            collider->CastRay(ray_cast_result, max_distance);
            if (ray_cast_result.hit_data.hit == true)
            {
                result.hit_list.push_back(ray_cast_result.hit_data);
            }
        }
        // sort the result list
        std::sort(result.hit_list.begin(), result.hit_list.end());
    }
}
