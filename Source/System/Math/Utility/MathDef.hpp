#pragma once
#include <codecvt>
#include <limits>

namespace CS460
{
    //32bit 
    using Real = float;

    namespace Math
    {
        constexpr Real PI         = (Real)3.14159265358979323846264;
        constexpr Real HALF_PI    = PI * (Real)0.5;
        constexpr Real QUARTER_PI = PI * (Real)0.25;
        constexpr Real TWO_PI     = PI * (Real)2;
        constexpr Real PI_DIV_2   = HALF_PI;
        constexpr Real PI_DIV_3   = PI / (Real)3;
        constexpr Real PI_DIV_4   = QUARTER_PI;
        constexpr Real PI_DIV_6   = PI / (Real)6;
        constexpr Real RADIAN     = PI / (Real)180;

        constexpr Real EPSILON         = (Real)0.00001;
        constexpr Real EPSILON_SQUARED = EPSILON * EPSILON;
        constexpr Real EPSILON_BIAS    = (Real)1.00001;

        constexpr Real REAL_MAX = FLT_MAX;
        constexpr Real REAL_MIN = FLT_MIN;

        constexpr Real REAL_POSITIVE_MAX = REAL_MAX; //go to +max
        constexpr Real REAL_POSITIVE_MIN = REAL_MIN; //near to 0
        constexpr Real REAL_NEGATIVE_MAX = -REAL_MAX; //go to -max
        constexpr Real REAL_NEGATIVE_MIN = -REAL_MIN; //near to 0

        constexpr Real ROOT_TWO        = (Real)1.41421356237309504880168;
        constexpr Real ROOT_THREE      = (Real)1.73205080756887729352744;
        constexpr Real ROOT_FIVE       = (Real)2.23606797749978969640917;
        constexpr Real ROOT_TEN        = (Real)3.16227766016837933199889;
        constexpr Real CUBE_ROOT_TWO   = (Real)1.25992104989487316476721;
        constexpr Real CUBE_ROOT_THREE = (Real)1.25992104989487316476721;
        constexpr Real FORTH_ROOT_TWO  = (Real)1.18920711500272106671749;

        constexpr Real LN_TWO   = (Real)0.69314718055994530941723;
        constexpr Real LN_THREE = (Real)1.09861228866810969139524;
        constexpr Real LN_TEN   = (Real)2.30258509299404568401799;

        const Real FNAN = std::numeric_limits<Real>::quiet_NaN();
        const Real INF  = std::numeric_limits<Real>::infinity();
    }
}
