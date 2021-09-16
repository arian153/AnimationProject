#pragma once

#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Primitive/Others/Ray.hpp"
#include <vector>

namespace CS460
{
    class ColliderPrimitive;

    class HitData
    {
    public:
        bool     operator<(const HitData& rhs) const;
        HitData& operator=(const HitData& rhs);

    public:
        ColliderPrimitive* collider = nullptr;
        bool               hit      = false;
        Real               t        = -1.0f;
        Vector3            intersection;
        Vector3            normal;
    };

    class RayCastResult
    {
    public:
        explicit       RayCastResult(const Ray& ray);
        RayCastResult& operator=(const RayCastResult& rhs);
        Vector3        GetLocalIntersection() const;
        Vector3        GetBodyIntersection() const;
        Vector3        GetWorldIntersection() const;
        Vector3        GetLocalNormal() const;
        Vector3        GetBodyNormal() const;
        Vector3        GetWorldNormal() const;

    public:
        Ray     ray; //global ray;
        HitData hit_data;
    };

    class RayIntersectionResult
    {
    public:
        explicit RayIntersectionResult(const Ray& ray);
        HitData  GetHitData(size_t i = 0);
        Vector3  GetLocalIntersection(size_t i = 0);
        Vector3  GetBodyIntersection(size_t i = 0);
        Vector3  GetWorldIntersection(size_t i = 0);
        Vector3  GetLocalNormal(size_t i = 0);
        Vector3  GetBodyNormal(size_t i = 0);
        Vector3  GetWorldNormal(size_t i = 0);

    public:
        Ray                  ray; //global ray
        std::vector<HitData> hit_list;
    };

    class RayTraceResult
    {
    public:
        explicit RayTraceResult(const Ray& initial_ray);
        HitData  GetHitData(size_t i = 0);
        Ray      GetRayData(size_t i = 0);
        Vector3  GetLocalIntersection(size_t i = 0);
        Vector3  GetBodyIntersection(size_t i = 0);
        Vector3  GetWorldIntersection(size_t i = 0);
        Vector3  GetLocalNormal(size_t i = 0);
        Vector3  GetBodyNormal(size_t i = 0);
        Vector3  GetWorldNormal(size_t i = 0);

    public:
        Ray                  initial_ray;
        std::vector<Ray>     ray_list;
        std::vector<HitData> hit_list;
    };
}
