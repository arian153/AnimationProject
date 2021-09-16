#pragma once
#include "MathDef.hpp"

namespace CS460
{
    namespace Math
    {
        inline bool IsValid(Real rhs)
        {
            return (!isnan(rhs)) && isfinite(rhs);
        }

        inline bool IsInfinite(Real rhs)
        {
            return !isfinite(rhs);
        }

        inline bool IsZero(Real rhs)
        {
            return (fabsf(rhs) < Math::EPSILON);
        }

        inline bool IsEqual(Real a, Real b)
        {
            return (fabsf(a - b) < Math::EPSILON);
        }

        inline bool IsNotEqual(Real a, Real b)
        {
            return !(fabsf(a - b) < Math::EPSILON);
        }

        inline bool IsLess(Real a, Real b)
        {
            return ((a + Math::EPSILON) < (b - Math::EPSILON));
        }

        inline bool IsGreator(Real a, Real b)
        {
            return ((a - Math::EPSILON) > (b + Math::EPSILON));
        }

        inline bool IsGreatorThanZero(Real a)
        {
            return (a >= Math::EPSILON);
        }

        inline Real InvSqrt(Real rhs)
        {
            return 1.f / sqrtf(rhs);
        }

        inline Real Signum(Real param)
        {
            if (IsZero(param))
            {
                return 0.0f;
            }
            if (param >= Math::EPSILON)
            {
                return +1.0f;
            }
            return -1.0f;
        }

        inline int Round(Real param)
        {
            return static_cast<int>(floorf(param + 0.5f));
        }

        inline Real DegreesToRadians(Real degrees)
        {
            return (degrees * Math::RADIAN);
        }

        inline Real RadiansToDegrees(Real radians)
        {
            return (radians / Math::RADIAN);
        }

        inline Real XYToRadian(Real x, Real y)
        {
            Real theta = atan2f(y, x);;
            if (theta > Math::PI)
                theta = theta + Math::TWO_PI;
            if (theta > Math::TWO_PI)
                theta -= Math::TWO_PI;
            if (theta < 0.0f)
                theta += Math::TWO_PI;
            return theta;
        }

        inline Real XYToDegree(Real x, Real y)
        {
            Real theta = atan2f(y, x);;
            if (theta > Math::PI)
                theta = theta + Math::TWO_PI;
            if (theta > Math::TWO_PI)
                theta -= Math::TWO_PI;
            if (theta < 0.0f)
                theta += Math::TWO_PI;
            return (theta / Math::RADIAN);
        }
    }
}
