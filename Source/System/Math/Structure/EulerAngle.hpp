#pragma once
#include "../Utility/MathDef.hpp"

namespace CS460
{
    class EulerAngle
    {
    public:
        explicit EulerAngle(Real x = 0.0f, Real y = 0.0f, Real z = 0.0f);
        EulerAngle(const EulerAngle& rhs);
        ~EulerAngle();
        EulerAngle& operator=(const EulerAngle& rhs);

    public:
        Real x_rotation, y_rotation, z_rotation;
    };
}
