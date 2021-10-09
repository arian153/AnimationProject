#include "Interpolation.hpp"

#include "Utility.hpp"

namespace CS460
{
    Real Lerp(Real start, Real end, Real t)
    {
        return (1.0f - t) * start + t * end;
    }

    Real Elerp(Real start, Real end, Real t)
    {
        return start * powf(end / start, t);
    }

    Vector3 Lerp(const Vector3& start, const Vector3& end, Real t)
    {
        return LinearInterpolation(start, end, t);
    }

    Quaternion Lerp(const Quaternion& start, const Quaternion& end, Real t)
    {
        return LinearInterpolation(start, end, t);
    }

    Quaternion Slerp(const Quaternion& start, const Quaternion& end, Real t)
    {
        return SphericalLinearInterpolation(start, end, t);
    }

    Quaternion Sterp(const Quaternion& start, const Quaternion& end, Real t, const Vector3& twist)
    {
        return SwingTwistInterpolation(start, end, twist, t);
    }
}
