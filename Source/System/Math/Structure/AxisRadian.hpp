#pragma once
#include "../Utility/MathDef.hpp"
#include "../Algebra/Vector3.hpp"
#include "../Utility/VectorDef.hpp"

namespace CS460
{
    class AxisRadian
    {
    public:
        explicit AxisRadian(const Vector3& axis = Math::Vector3::Y_AXIS, Real radian = 0.0f);
        AxisRadian(const AxisRadian& rhs);
        ~AxisRadian();

        AxisRadian& operator=(const AxisRadian& rhs);

    public:
        Vector3 axis;
        Real    radian;
    };
}
