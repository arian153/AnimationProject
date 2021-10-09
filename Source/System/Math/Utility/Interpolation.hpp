#pragma once
#include "MathDef.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../Algebra/Vector3.hpp"

namespace CS460
{
    //scalar
    Real Lerp(Real start, Real end, Real t);
    Real Elerp(Real start, Real end, Real t);

    //vector
    Vector3 Lerp(const Vector3& start, const Vector3& end, Real t);

    //quaternion
    Quaternion Lerp(const Quaternion& start, const Quaternion& end, Real t);
    Quaternion Slerp(const Quaternion& start, const Quaternion& end, Real t);
    Quaternion Sterp(const Quaternion& start, const Quaternion& end, Real t, const Vector3& twist = Math::Vector3::UP);
}
