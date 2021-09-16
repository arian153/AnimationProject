#pragma once
#include "../../Algebra/Vector3.hpp"
#include "../../Algebra/Quaternion.hpp"

namespace CS460
{
    class Segment
    {
    public:
        Segment();
        ~Segment();
        explicit Segment(const Vector3& start, const Vector3& end);
        Segment(const Segment& rhs);

        Vector3 Position() const;
        Vector3 Direction() const;
        Vector3 Center() const;
        Vector3 ClosestPoint(const Vector3& point) const;

        Real Length() const;
        Real LengthSquared() const;
        Real Distance(const Vector3& point, Real& t) const;
        Real DistanceSquared(const Vector3& point, Real& t) const;

    public:
        Segment& operator=(const Segment& rhs);
        bool     operator ==(const Segment& rhs) const;
        bool     operator !=(const Segment& rhs) const;

    public:
        Vector3 start;
        Vector3 end = Math::Vector3::X_AXIS;
    };
}
