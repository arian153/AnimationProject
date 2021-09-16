#include "SupportPoint.hpp"

namespace CS460
{
    SupportPoint::SupportPoint()
        : index(0)
    {
    }

    SupportPoint::SupportPoint(const Vector3& global, const Vector3& local1, const Vector3& local2, size_t idx)
        : global(global), local_a(local1), local_b(local2), index(idx)
    {
    }

    SupportPoint::SupportPoint(const SupportPoint& rhs)
        : global(rhs.global), local_a(rhs.local_a), local_b(rhs.local_b), index(rhs.index)
    {
    }

    SupportPoint::~SupportPoint()
    {
    }

    SupportPoint& SupportPoint::operator=(const SupportPoint& rhs)
    {
        if (this != &rhs)
        {
            global = rhs.global;
            local_a = rhs.local_a;
            local_b = rhs.local_b;
            index  = rhs.index;
        }
        return *this;
    }

    Vector3& SupportPoint::operator[](size_t index)
    {
        return (&global)[index];
    }

    Vector3 SupportPoint::operator[](size_t index) const
    {
        return (&global)[index];
    }

    bool SupportPoint::operator==(const SupportPoint& rhs)
    {
        return (this->global == rhs.global);
    }

    bool SupportPoint::operator==(const SupportPoint& rhs) const
    {
        return (this->global == rhs.global);
    }

    bool SupportPoint::IsValid() const
    {
        return global.IsValid();
    }
}
