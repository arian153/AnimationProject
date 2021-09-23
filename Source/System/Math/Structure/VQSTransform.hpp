#pragma once
#include "../Algebra/Vector3.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../Algebra/Matrix44.hpp"

namespace CS460
{
    class VQSTransform
    {
    public:
        VQSTransform();
        VQSTransform(const VQSTransform& vqs);
        VQSTransform(const Vector3& pos, const Quaternion& rot, Real s = 1.0f);
        ~VQSTransform();

        Matrix44 ToMatrix() const;

        VQSTransform Identity();
        VQSTransform Inverse() const;

        Vector3 ApplyTransform(const Vector3& point) const;
        Vector3 ApplyInverse(const Vector3& point) const;

    public:
        VQSTransform& operator=(const VQSTransform& vqs);
        VQSTransform  operator +(const VQSTransform& vqs) const;
        VQSTransform  operator *(Real real) const;
        VQSTransform  operator *(const VQSTransform& vqs) const;

        Vector3              operator *(const Vector3& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const VQSTransform& vqs);
        friend VQSTransform  operator*(Real real, const VQSTransform& vqs);

    private:
        Vector3    position;
        Real       scale = 1.0f;
        Quaternion rotation;
    };
}
