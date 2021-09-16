#pragma once
#include "../../Algebra/Vector3.hpp"
#include "../../Algebra/Quaternion.hpp"

namespace CS460
{
    class Ray;

    class Plane
    {
    public:
        Plane();
        ~Plane();
        Plane(Real a, Real b, Real c, Real d);
        Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2);
        Plane(const Plane& rhs);
        Plane& operator=(const Plane& rhs);
        bool   operator==(const Plane& rhs) const;

        void Set(Real a, Real b, Real c, Real d);
        void Set(const Vector3& normal, Real offset);
        void Set(const Vector3& p0, const Vector3& p1, const Vector3& p2);

        Real    Distance(const Vector3& point) const;
        Real    PlaneTest(const Vector3& point) const;
        Vector3 ClosestPoint(const Vector3& point) const;
        Vector3 Normal() const;

        bool IntersectRay(const Ray& ray, Real& t) const;

        void SetNormalize();

    public:
        Real a, b, c, d;
    };
}
