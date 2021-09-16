#include "Vector3Pair.hpp"

namespace CS460
{
    Vector3Pair::Vector3Pair(const Vector3& a, const Vector3& b)
        : a(a), b(b)
    {
    }

    Vector3Pair::Vector3Pair(const Vector3Pair& rhs)
        : a(rhs.a), b(rhs.b)
    {
    }

    Vector3Pair& Vector3Pair::operator=(const Vector3Pair& rhs)
    {
        if (this != &rhs)
        {
            a = rhs.a;
            b = rhs.b;
        }
        return *this;
    }
}