#include "BezierCurve.hpp"

#include "../Utility/Utility.hpp"

namespace CS460
{
    CubicBezierCurve::CubicBezierCurve()
    {
    }

    CubicBezierCurve::~CubicBezierCurve()
    {
    }

    Vector3 CubicBezierCurve::Interpolate(Real u) const
    {
        u = Math::Clamp(u, 0.0f, 1.0f);

        Real u3 = u * u * u;
        Real u2 = u * u;

        return u3 * (-p0 + 3.0f * p1 - 3.0f * p2 + p3)
                + u2 * (3.0f * p0 - 6.0f * p1 + 3.0f * p2)
                + u * (-3.0f * p0 + 3.0f * p1) + p0;
    }

    BezierCurve::BezierCurve()
    {
    }

    BezierCurve::~BezierCurve()
    {
    }
}
