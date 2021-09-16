#pragma once
#include "MathDef.hpp"


namespace CS460
{
    class Vector3;
    class Vector3Pair;

    namespace Math
    {
        Real        LinearInterpolation(Real v0, Real v1, Real t);
        Real        SmoothStart(Real t);
        Real        SmoothStop(Real t);
        Vector3Pair GetTangentUsingQuaternion(const CS460::Vector3& normal);
        bool SolveQuadratic(Real a, Real b, Real c, Real& result1, Real& result2);

        template <typename T>
        T Min(const T& a, const T& b);

        template <typename T>
        T Max(const T& a, const T& b);

        template <typename T>
        T Clamp(const T& x, const T& low, const T& high);
    }
}

#include "Utility.inl"
// ReSharper disable once CppUnusedIncludeDirective
#include "UtilityTemplate.cpp"
