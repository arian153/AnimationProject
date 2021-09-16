#pragma once
#include "../Algebra/Vector3.hpp"
#include "../Utility/VectorDef.hpp"

namespace CS460
{
    class Quaternion;

    class Basis
    {
    public:
        Basis();
        explicit Basis(const Vector3& a, const Vector3& b, const Vector3& c);
        Basis(const Basis& rhs);
        Basis& operator=(const Basis& rhs);
        ~Basis();

        void CalculateBasis(const Vector3& normal);
        void CalculateBasisApprox(const Vector3& normal);
        void CalculateBasisQuaternion(const Vector3& normal);
        void CalculateBasis(const Vector3& a, const Vector3& b);
        void Rotate(const Quaternion& orientation);
        void Normalize();

    public:
        Vector3 i = Math::Vector3::X_AXIS;
        Vector3 j = Math::Vector3::Y_AXIS;
        Vector3 k = Math::Vector3::Z_AXIS;
    };
}
