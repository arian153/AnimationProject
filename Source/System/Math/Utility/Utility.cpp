#include "Utility.hpp"
#include "../Algebra/Vector3.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../Structure/Vector3Pair.hpp"
#include "VectorDef.hpp"

namespace CS460
{
    namespace Math
    {
        Real LinearInterpolation(Real v0, Real v1, Real t)
        {
            return (1.0f - t) * v0 + t * v1;
        }

        Real SmoothStart(Real t)
        {
            return t * t;
        }

        Real SmoothStop(Real t)
        {
            return 1.0f - ((1.0f - t) * (1.0f - t));
        }

        Vector3Pair GetTangentUsingQuaternion(const CS460::Vector3& normal)
        {
            Quaternion axis_to_normal(Math::Vector3::Y_AXIS, normal);
            return Vector3Pair(axis_to_normal.Rotate(Math::Vector3::X_AXIS), axis_to_normal.Rotate(Math::Vector3::Z_AXIS));
        }

        bool SolveQuadratic(Real a, Real b, Real c, Real& result1, Real& result2)
        {
            if (IsZero(a))
            {
                if (IsZero(b))
                {
                    result1 = -c;
                    result2 = -c;
                }
                else
                {
                    result1 = -c / b;
                    result2 = -c / b;
                }

                return false;
            }

            Real discriminant = (b * b) - (4.0f * a * c);

            if (discriminant > 0.0f)
            {
                result1 = (-b + sqrtf(discriminant)) / (2.0f * a);
                result2 = (-b - sqrtf(discriminant)) / (2.0f * a);
            }
            else if (IsZero(discriminant) == true)
            {
                Real result = -b / (2.0f * a);
                result1 = result;
                result2 = result;
            }
            else
            {
                Real real = -b / (2.0f * a);
                Real imaginary = sqrtf(-discriminant) / (2.0f * a);
                result1 = real;
                result2 = imaginary;
                return false;
            }

            return true;
        }
    }
}
