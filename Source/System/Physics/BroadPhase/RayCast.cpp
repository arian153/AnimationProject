#include "RayCast.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace CS460
{
    bool HitData::operator<(const HitData& rhs) const
    {
        return t > rhs.t;
    }

    HitData& HitData::operator=(const HitData& rhs)
    {
        if (this != &rhs)
        {
            collider     = rhs.collider;
            hit          = rhs.hit;
            t            = rhs.t;
            intersection = rhs.intersection;
            normal       = rhs.normal;
        }
        return *this;
    }

    RayCastResult::RayCastResult(const Ray& ray)
        : ray(ray)
    {
    }

    RayCastResult& RayCastResult::operator=(const RayCastResult& rhs)
    {
        if (this != &rhs)
        {
            ray      = rhs.ray;
            hit_data = rhs.hit_data;
        }
        return *this;
    }

    Vector3 RayCastResult::GetLocalIntersection() const
    {
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        Ray                body_ray(body->WorldToLocalPoint(ray.position), body->WorldToLocalVector(ray.direction));
        Ray                local_ray(collider->WorldToLocalPoint(body_ray.position), collider->WorldToLocalVector(body_ray.direction));
        return local_ray.position + local_ray.direction * hit_data.t;
    }

    Vector3 RayCastResult::GetBodyIntersection() const
    {
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        Ray                body_ray(body->WorldToLocalPoint(ray.position), body->WorldToLocalVector(ray.direction));
        return body_ray.position + body_ray.direction * hit_data.t;
    }

    Vector3 RayCastResult::GetWorldIntersection() const
    {
        return ray.position + ray.direction * hit_data.t;
    }

    Vector3 RayCastResult::GetLocalNormal() const
    {
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        return collider->WorldToLocalVector(body->WorldToLocalVector(hit_data.normal));
    }

    Vector3 RayCastResult::GetBodyNormal() const
    {
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        return body->WorldToLocalVector(hit_data.normal);
    }

    Vector3 RayCastResult::GetWorldNormal() const
    {
        return hit_data.normal;
    }

    RayIntersectionResult::RayIntersectionResult(const Ray& ray)
        : ray(ray)
    {
    }

    HitData RayIntersectionResult::GetHitData(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        return hit_list.at(i);
    }

    Vector3 RayIntersectionResult::GetLocalIntersection(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto               hit_data = hit_list.at(i);
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        Ray                body_ray(body->WorldToLocalPoint(ray.position), body->WorldToLocalVector(ray.direction));
        Ray                local_ray(collider->WorldToLocalPoint(body_ray.position), collider->WorldToLocalVector(body_ray.direction));
        return local_ray.position + local_ray.direction * hit_data.t;
    }

    Vector3 RayIntersectionResult::GetBodyIntersection(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto               hit_data = hit_list.at(i);
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        Ray                body_ray(body->WorldToLocalPoint(ray.position), body->WorldToLocalVector(ray.direction));
        return body_ray.position + body_ray.direction * hit_data.t;
    }

    Vector3 RayIntersectionResult::GetWorldIntersection(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto hit_data = hit_list.at(i);
        return hit_data.intersection;
    }

    Vector3 RayIntersectionResult::GetLocalNormal(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto               hit_data = hit_list.at(i);
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        return collider->WorldToLocalVector(body->WorldToLocalVector(hit_data.normal));
    }

    Vector3 RayIntersectionResult::GetBodyNormal(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto               hit_data = hit_list.at(i);
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        return body->WorldToLocalVector(hit_data.normal);
    }

    Vector3 RayIntersectionResult::GetWorldNormal(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto hit_data = hit_list.at(i);
        return hit_data.normal;
    }

    RayTraceResult::RayTraceResult(const Ray& initial_ray)
        : initial_ray(initial_ray)
    {
        ray_list.push_back(initial_ray);
    }

    HitData RayTraceResult::GetHitData(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        return hit_list.at(i);
    }

    Ray RayTraceResult::GetRayData(size_t i)
    {
        if (i >= ray_list.size())
        {
            i = ray_list.size() - 1;
        }
        return ray_list.at(i);
    }

    Vector3 RayTraceResult::GetLocalIntersection(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto               hit_data = hit_list.at(i);
        auto               ray      = ray_list.at(i);
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        Ray                body_ray(body->WorldToLocalPoint(ray.position), body->WorldToLocalVector(ray.direction));
        Ray                local_ray(collider->WorldToLocalPoint(body_ray.position), collider->WorldToLocalVector(body_ray.direction));
        return local_ray.position + local_ray.direction * hit_data.t;
    }

    Vector3 RayTraceResult::GetBodyIntersection(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto               hit_data = hit_list.at(i);
        auto               ray      = ray_list.at(i);
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        Ray                body_ray(body->WorldToLocalPoint(ray.position), body->WorldToLocalVector(ray.direction));
        return body_ray.position + body_ray.direction * hit_data.t;
    }

    Vector3 RayTraceResult::GetWorldIntersection(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto hit_data = hit_list.at(i);
        return hit_data.intersection;
    }

    Vector3 RayTraceResult::GetLocalNormal(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto               hit_data = hit_list.at(i);
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        return collider->WorldToLocalVector(body->WorldToLocalVector(hit_data.normal));
    }

    Vector3 RayTraceResult::GetBodyNormal(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto               hit_data = hit_list.at(i);
        ColliderPrimitive* collider = hit_data.collider;
        RigidBody*         body     = collider->GetRigidBody();
        return body->WorldToLocalVector(hit_data.normal);
    }

    Vector3 RayTraceResult::GetWorldNormal(size_t i)
    {
        if (i >= hit_list.size())
        {
            i = hit_list.size() - 1;
        }
        auto hit_data = hit_list.at(i);
        return hit_data.normal;
    }
}
