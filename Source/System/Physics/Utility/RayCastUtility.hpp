#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Primitive/Others/Ray.hpp"
#include <vector>

namespace CS460
{
    class Primitive;

    class PrimitiveHitData
    {
    public:
        bool              operator<(const PrimitiveHitData& rhs) const;
        PrimitiveHitData& operator=(const PrimitiveHitData& rhs);

    public:
        Primitive* primitive = nullptr;
        bool       hit       = false;
        Real       t         = -1.0f;
    };

    class PrimitiveRayCastResult
    {
    public:
        explicit                PrimitiveRayCastResult(const Ray& ray);
        PrimitiveRayCastResult& operator=(const PrimitiveRayCastResult& rhs);

        Vector3 GetLocalIntersection() const;
        Vector3 GetWorldIntersection() const;
        Vector3 GetLocalNormal() const;
        Vector3 GetWorldNormal() const;

    public:
        Ray              ray; //global ray;
        PrimitiveHitData hit_data;
        Vector3          intersection;
        Vector3          normal;
    };

    class PrimitiveRayIntersectionResult
    {
    public:
        explicit         PrimitiveRayIntersectionResult(const Ray& ray);
        PrimitiveHitData GetHitData(size_t i = 0);
        Vector3          GetLocalIntersection(size_t i = 0);
        Vector3          GetWorldIntersection(size_t i = 0);
        Vector3          GetLocalNormal(size_t i = 0);
        Vector3          GetWorldNormal(size_t i = 0);

    public:
        Ray                           ray; //global ray
        std::vector<PrimitiveHitData> hit_list;
    };
}
