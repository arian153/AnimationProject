#pragma once
#include <vector>

#include "../Algebra/Vector3.hpp"

namespace CS460
{
    class CubicBezierCurve
    {
    public:
        CubicBezierCurve();
        ~CubicBezierCurve();

        Vector3 Interpolate(Real u) const;

    public:
        Vector3              p0, p1, p2, p3;
        std::vector<Vector3> interpolated_points;
    };

    class BezierCurve
    {
    public:
        BezierCurve();
        ~BezierCurve();

    public:
        std::vector<Vector3> control_points;
        std::vector<Vector3> interpolated_points;
    };
}
