#include "MathDef.hpp"

namespace CS460
{
    namespace Math 
    {
        template<typename T>
        T Min(const T& a, const T& b)
        {
            return a < b ? a : b;
        }

        template<typename T>
        T Max(const T& a, const T& b)
        {
            return a > b ? a : b;
        }

        template<typename T>
        T Clamp(const T& x, const T& low, const T& high)
        {
            return x < low ? low : (x > high ? high : x);
        }

    }
}