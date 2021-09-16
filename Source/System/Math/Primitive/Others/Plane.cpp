#include "Plane.hpp"
#include "../../Utility/Utility.hpp"
#include "Ray.hpp"

namespace CS460
{
    Plane::Plane()
        : a(1.0f), b(0.0f), c(0.0f), d(0.0f)
    {
    }

    Plane::~Plane()
    {
    }

    Plane::Plane(Real a, Real b, Real c, Real d)
    {
        Set(a, b, c, d);
    }

    Plane::Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
        Set(p0, p1, p2);
    }

    Plane::Plane(const Plane& rhs)
    {
        a = rhs.a;
        b = rhs.b;
        c = rhs.c;
        d = rhs.d;
    }

    Plane& Plane::operator=(const Plane& rhs)
    {
        if (this != &rhs)
        {
            a = rhs.a;
            b = rhs.b;
            c = rhs.c;
            d = rhs.d;
        }
        return *this;
    }

    bool Plane::operator==(const Plane& rhs) const
    {
        return Math::IsEqual(a, rhs.a)
                && Math::IsEqual(b, rhs.b)
                && Math::IsEqual(c, rhs.c)
                && Math::IsEqual(d, rhs.d);
    }

    void Plane::Set(Real _a, Real _b, Real _c, Real _d)
    {
        Real length_squared = _a * _a + _b * _b + _c * _c;
        if (Math::IsZero(length_squared) == true)
        {
            a = 0.0f;
            b = 0.0f;
            c = 0.0f;
            d = 0.0f;
        }
        else
        {
            Real inv_sqrt = Math::InvSqrt(length_squared);
            a             = _a * inv_sqrt;
            b             = _b * inv_sqrt;
            c             = _c * inv_sqrt;
            d             = _d * inv_sqrt;
        }
    }

    void Plane::Set(const Vector3& normal, Real offset)
    {
        if (Math::IsZero(normal.LengthSquared()) == true)
        {
            a = 0.0f;
            b = 0.0f;
            c = 0.0f;
            d = 0.0f;
        }
        else
        {
            Vector3 n = normal.Normalize();
            a         = n.x;
            b         = n.y;
            c         = n.z;
            d         = offset / n.Length();
        }
    }

    void Plane::Set(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
        Vector3 u = p1 - p0;
        Vector3 v = p2 - p0;
        Vector3 w = u.CrossProduct(v);
        if (Math::IsZero(w.LengthSquared()) == true)
        {
            a = 0.0f;
            b = 0.0f;
            c = 0.0f;
            d = 0.0f;
        }
        else
        {
            Vector3 n = w.Normalize();
            a         = n.x;
            b         = n.y;
            c         = n.z;
            d         = -n.DotProduct(p0);
        }
    }

    Real Plane::Distance(const Vector3& point) const
    {
        Vector3 normal(a, b, c);
        return fabsf(normal.DotProduct(point) + d);
    }

    Real Plane::PlaneTest(const Vector3& point) const
    {
        Vector3 normal(a, b, c);
        return normal.DotProduct(point) + d;
    }

    Vector3 Plane::ClosestPoint(const Vector3& point) const
    {
        Vector3 normal(a, b, c);
        return point - PlaneTest(point) * normal;
    }

    Vector3 Plane::Normal() const
    {
        return Vector3(a, b, c).Normalize();
    }

    bool Plane::IntersectRay(const Ray& ray, Real& t) const
    {
        Vector3 n     = Normal();
        Real    denom = DotProduct(n, ray.direction);
        if (!Math::IsZero(denom))
        {
            t = -(DotProduct(n, ray.position) + d) / denom;
            return true;
        }
        return false;
    }

    void Plane::SetNormalize()
    {
        Real length = sqrtf(a * a + b * b + c * c);
        // Prevent divide by zero
        length = length > 0.0f ? 1.0f / length : 0.0f;
        a *= length;
        b *= length;
        c *= length;
        d *= length;
    }
}
