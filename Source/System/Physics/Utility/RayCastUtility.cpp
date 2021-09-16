#include "RayCastUtility.hpp"
#include "../../Math/Primitive/Primitive.hpp"

namespace CS460
{
    bool PrimitiveHitData::operator<(const PrimitiveHitData& rhs) const
    {
        return t > rhs.t;
    }

    PrimitiveHitData& PrimitiveHitData::operator=(const PrimitiveHitData& rhs)
    {
        if (this != &rhs)
        {
            primitive = rhs.primitive;
            hit       = rhs.hit;
            t         = rhs.t;
        }
        return *this;
    }

    PrimitiveRayCastResult::PrimitiveRayCastResult(const Ray& ray)
        : ray(ray)
    {
    }

    PrimitiveRayCastResult& PrimitiveRayCastResult::operator=(const PrimitiveRayCastResult& rhs)
    {
        if (this != &rhs)
        {
            ray          = rhs.ray;
            hit_data     = rhs.hit_data;
            intersection = rhs.intersection;
            normal       = rhs.normal;
        }
        return *this;
    }

    Vector3 PrimitiveRayCastResult::GetLocalIntersection() const
    {
        Primitive* primitive = hit_data.primitive;
        Ray        local_ray;
        local_ray.position  = primitive->WorldToLocalPoint(ray.position);
        local_ray.direction = primitive->WorldToLocalVector(ray.direction);
        return local_ray.position + local_ray.direction * hit_data.t;
    }

    Vector3 PrimitiveRayCastResult::GetWorldIntersection() const
    {
        return ray.position + ray.direction * hit_data.t;
    }

    Vector3 PrimitiveRayCastResult::GetLocalNormal() const
    {
        Primitive* primitive = hit_data.primitive;
        Ray        local_ray;
        local_ray.position  = primitive->WorldToLocalPoint(ray.position);
        local_ray.direction = primitive->WorldToLocalVector(ray.direction);
        return primitive->GetNormal(local_ray.position + local_ray.direction * hit_data.t);
    }

    Vector3 PrimitiveRayCastResult::GetWorldNormal() const
    {
        Primitive* primitive = hit_data.primitive;
        Ray        local_ray;
        local_ray.position   = primitive->WorldToLocalPoint(ray.position);
        local_ray.direction  = primitive->WorldToLocalVector(ray.direction);
        Vector3 local_normal = primitive->GetNormal(local_ray.position + local_ray.direction * hit_data.t);
        return primitive->LocalToWorldVector(local_normal);
    }

    PrimitiveRayIntersectionResult::PrimitiveRayIntersectionResult(const Ray& ray)
        : ray(ray)
    {
    }

    PrimitiveHitData PrimitiveRayIntersectionResult::GetHitData(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        return hit_list.at(i);
    }

    Vector3 PrimitiveRayIntersectionResult::GetLocalIntersection(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto       hit_data  = hit_list.at(i);
        Primitive* primitive = hit_data.primitive;
        Ray        local_ray;
        local_ray.position  = primitive->WorldToLocalPoint(ray.position);
        local_ray.direction = primitive->WorldToLocalVector(ray.direction);
        return local_ray.position + local_ray.direction * hit_data.t;
    }

    Vector3 PrimitiveRayIntersectionResult::GetWorldIntersection(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto hit_data = hit_list.at(i);
        return ray.position + ray.direction * hit_data.t;
    }

    Vector3 PrimitiveRayIntersectionResult::GetLocalNormal(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto       hit_data  = hit_list.at(i);
        Primitive* primitive = hit_data.primitive;
        Ray        local_ray;
        local_ray.position  = primitive->WorldToLocalPoint(ray.position);
        local_ray.direction = primitive->WorldToLocalVector(ray.direction);
        return primitive->GetNormal(local_ray.position + local_ray.direction * hit_data.t);
    }

    Vector3 PrimitiveRayIntersectionResult::GetWorldNormal(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto       hit_data  = hit_list.at(i);
        Primitive* primitive = hit_data.primitive;
        Ray        local_ray;
        local_ray.position   = primitive->WorldToLocalPoint(ray.position);
        local_ray.direction  = primitive->WorldToLocalVector(ray.direction);
        Vector3 local_normal = primitive->GetNormal(local_ray.position + local_ray.direction * hit_data.t);
        return primitive->LocalToWorldVector(local_normal);
    }
}
