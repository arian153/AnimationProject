#pragma once
#include "../../Math/Algebra/Vector3.hpp"

namespace CS460
{
    class SupportPoint
    {
    public:
        SupportPoint();
        SupportPoint(const Vector3& global, const Vector3& local1, const Vector3& local2, size_t idx = 0);
        SupportPoint(const SupportPoint& rhs);
        ~SupportPoint();

        SupportPoint& operator=(const SupportPoint& rhs);
        Vector3&      operator[](size_t index);
        Vector3       operator[](size_t index) const;
        bool          operator==(const SupportPoint& rhs);
        bool          operator==(const SupportPoint& rhs) const;
        bool IsValid() const;

    public:
        Vector3 global;
        Vector3 local_a;
        Vector3 local_b;
        size_t  index;
    };
}
