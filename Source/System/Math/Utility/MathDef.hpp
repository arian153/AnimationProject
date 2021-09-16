#pragma once
#include <codecvt>

namespace CS460
{
    //32bit 
    using Real = float;

    namespace Math
    {
        constexpr Real PI = 3.14159265358979323846264f;
        constexpr Real HALF_PI = PI * 0.5f;
        constexpr Real QUARTER_PI = PI * 0.25f;
        constexpr Real TWO_PI = PI * 2.f;
        constexpr Real PI_DIV_2 = HALF_PI;
        constexpr Real PI_DIV_3 = PI / 3;
        constexpr Real PI_DIV_4 = QUARTER_PI;
        constexpr Real PI_DIV_6 = PI / 6;
        constexpr Real RADIAN = PI / 180.0f;

        constexpr Real EPSILON = 0.00001f;
        constexpr Real EPSILON_SQUARED = EPSILON * EPSILON;
        constexpr Real EPSILON_BIAS = 1.00001f;

        constexpr Real REAL_MAX = 3.402823466e+38F;
        constexpr Real REAL_MIN = 1.175494351e-38F;

        constexpr Real REAL_POSITIVE_MAX = REAL_MAX; //go to +max
        constexpr Real REAL_POSITIVE_MIN = REAL_MIN; //near to 0
        constexpr Real REAL_NEGATIVE_MAX = -REAL_MAX; //go to -max
        constexpr Real REAL_NEGATIVE_MIN = -REAL_MIN; //near to 0

        constexpr Real ROOT_TWO = 1.41421356237309504880168f;
        constexpr Real ROOT_THREE = 1.73205080756887729352744f;
        constexpr Real ROOT_FIVE = 2.23606797749978969640917f;
        constexpr Real ROOT_TEN = 3.16227766016837933199889f;
        constexpr Real CUBE_ROOT_TWO = 1.25992104989487316476721f;
        constexpr Real CUBE_ROOT_THREE = 1.25992104989487316476721f;
        constexpr Real FORTH_ROOT_TWO = 1.18920711500272106671749f;

        constexpr Real LN_TWO = 0.69314718055994530941723f;
        constexpr Real LN_THREE = 1.09861228866810969139524f;
        constexpr Real LN_TEN = 2.30258509299404568401799f;

        const Real FNAN = std::numeric_limits<Real>::quiet_NaN();
        const Real INF = std::numeric_limits<Real>::infinity();
    }

}