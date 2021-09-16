#include "Segment.hpp"
#include "../../Utility/Utility.hpp"

namespace CS460
{
    Segment::Segment()
    {
    }

    Segment::~Segment()
    {
    }

    Segment::Segment(const Vector3& start, const Vector3& end)
        : start(start), end(end)
    {
    }

    Segment::Segment(const Segment& rhs)
        : start(rhs.start), end(rhs.end)
    {
    }

    Vector3 Segment::Position() const
    {
        return start;
    }

    Vector3 Segment::Direction() const
    {
        return end - start;
    }

    Vector3 Segment::Center() const
    {
        return 0.5f * (start + end);
    }

    Vector3 Segment::ClosestPoint(const Vector3& point) const
    {
        Vector3 n      = (end - start).Normalize();
        Real    length = (end - start).Length();
        Real    u      = (end - point).DotProduct(n) / length;
        Real    v      = (point - start).DotProduct(n) / length;
        if (u <= 0.0f)
        {
            return end;
        }
        if (v <= 0.0f)
        {
            return start;
        }
        return u * start + v * end;
    }

    Real Segment::Length() const
    {
        return (end - start).Length();
    }

    Real Segment::LengthSquared() const
    {
        return (end - start).LengthSquared();
    }

    Real Segment::Distance(const Vector3& point, Real& t) const
    {
        return sqrtf(DistanceSquared(point, t));
    }

    Real Segment::DistanceSquared(const Vector3& point, Real& t) const
    {
        Vector3 w    = point - start;
        Vector3 dir  = end - start;
        Real    proj = w.DotProduct(dir);
        if (proj <= 0)
        {
            t = 0.0f;
            return w.DotProduct(w);
        }
        Real vsq = dir.DotProduct(dir);
        if (proj >= vsq)
        {
            t = 1.0f;
            return w.DotProduct(w) - 2.0f * proj + vsq;
        }
        t = proj / vsq;
        return w.DotProduct(w) - t * proj;
    }


    Segment& Segment::operator=(const Segment& rhs)
    {
        if (this != &rhs)
        {
            start = rhs.start;
            end   = rhs.end;
        }
        return *this;
    }

    bool Segment::operator==(const Segment& rhs) const
    {
        return start == rhs.start && end == rhs.end;
    }

    bool Segment::operator!=(const Segment& rhs) const
    {
        return !(start == rhs.start && end == rhs.end);
    }
}
