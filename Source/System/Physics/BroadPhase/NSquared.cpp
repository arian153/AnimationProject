#include "NSquared.hpp"
#include "../../Core/Utility/CoreUtility.hpp"
#include "..//ColliderPrimitive/ColliderPrimitive.hpp"
#include <algorithm>
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    NSquared::NSquared()
    {
    }

    NSquared::~NSquared()
    {
    }

    void NSquared::Initialize()
    {
    }

    void NSquared::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        for (auto& aabb : m_aabb_list)
        {
            if (aabb != nullptr)
            {
                aabb->GetCollider()->UpdateBoundingVolume();
            }
        }
    }

    void NSquared::Shutdown()
    {
        Release();
        Clear();
    }

    void NSquared::Copy(BroadPhase* other)
    {
        for (auto& aabb : m_aabb_list)
        {
            other->Add(aabb);
        }
    }

    void NSquared::Add(BoundingAABB* aabb)
    {
        m_aabb_list.push_back(aabb);
    }

    void NSquared::Remove(BoundingAABB* aabb)
    {
        m_aabb_list.remove(aabb);
    }

    void NSquared::Clear()
    {
        m_aabb_list.clear();
    }

    void NSquared::Release()
    {
        for (auto& aabb : m_aabb_list)
        {
            if (aabb != nullptr)
            {
                delete aabb;
                aabb = nullptr;
            }
        }
    }

    void NSquared::Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color)
    {
        for (auto& aabb : m_aabb_list)
        {
            if (aabb->GetCollider() != nullptr && primitive_color.b_flag)
            {
                aabb->GetCollider()->Draw(primitive_renderer, eRenderingMode::Line, primitive_color.color);
            }

            if (broad_phase_color.b_flag)
            {
                primitive_renderer->DrawBox(aabb->Center(), Quaternion(), aabb->Size(), eRenderingMode::Line, broad_phase_color.color);
            }
        }
    }

    void NSquared::ComputePairs(PotentialPairs& result)
    {
        result.clear();
        auto aabb_end = m_aabb_list.end();
        for (auto it = m_aabb_list.begin(); it != aabb_end; ++it)
        {
            auto jt_begin = it;
            for (auto jt = jt_begin; jt != aabb_end; ++jt)
            {
                if (it != jt)
                {
                    BoundingAABB*      aabb_a     = (*it);
                    BoundingAABB*      aabb_b     = (*jt);
                    ColliderPrimitive* collider_a = aabb_a->GetCollider();
                    ColliderPrimitive* collider_b = aabb_b->GetCollider();
                    if (aabb_a->Intersect(aabb_b))
                    {
                        result.emplace_back(collider_a, collider_b);
                    }
                }
            }
        }
    }

    ColliderPrimitive* NSquared::Pick(const Vector3& point) const
    {
        for (auto& aabb : m_aabb_list)
        {
            if (aabb->Contains(point) == true)
            {
                return aabb->GetCollider();
            }
        }
        return nullptr;
    }

    void NSquared::Query(const BoundingAABB& aabb, std::vector<ColliderPrimitive*>& output) const
    {
        for (auto& collider_aabb : m_aabb_list)
        {
            if (collider_aabb->Intersect(aabb) == true)
            {
                output.push_back(collider_aabb->GetCollider());
            }
        }
    }

    void NSquared::CastRay(RayCastResult& result, Real max_distance) const
    {
        Ray ray = result.ray;
        // test AABBs for candidates
        std::vector<ColliderPrimitive*> candidate_list;
        candidate_list.reserve(m_aabb_list.size());
        Real t = Math::REAL_POSITIVE_MAX;
        for (auto& aabb : m_aabb_list)
        {
            if (aabb->TestRayIntersection(ray, t, max_distance) == true)
            {
                candidate_list.push_back(aabb->GetCollider());
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

    void NSquared::IntersectRay(RayIntersectionResult& result, Real max_distance) const
    {
        Ray ray = result.ray;
        // test AABBs for candidates
        std::vector<ColliderPrimitive*> candidate_list;
        candidate_list.reserve(m_aabb_list.size());
        Real t = Math::REAL_POSITIVE_MAX;
        for (auto& aabb : m_aabb_list)
        {
            if (aabb->TestRayIntersection(ray, t, max_distance) == true)
            {
                candidate_list.push_back(aabb->GetCollider());
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

    void NSquared::TraceRay(RayTraceResult& result, Real max_distance, size_t reflect_count) const
    {
        E5_UNUSED_ARGS(result, max_distance, reflect_count);
    }
}
