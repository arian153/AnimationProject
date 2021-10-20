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

        void Set(const Vector3& pos, const Quaternion& rot, Real s);
        Matrix44 ToMatrix() const;

        VQSTransform Identity();
        VQSTransform Inverse() const;

        Vector3 ApplyTransform(const Vector3& point) const;
        Vector3 ApplyInverse(const Vector3& point) const;

        friend VQSTransform Concatenate(const VQSTransform& a, const VQSTransform& b);
        friend VQSTransform Interpolation(const VQSTransform& start, const VQSTransform& end, Real t);
        friend Matrix44 Concatenate(const Matrix44& a, const Matrix44& b);
     

    public:
        VQSTransform& operator=(const VQSTransform& vqs);
        VQSTransform  operator +(const VQSTransform& vqs) const;
        VQSTransform  operator *(Real real) const;
        VQSTransform  operator *(const VQSTransform& vqs) const;

        Vector3              operator *(const Vector3& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const VQSTransform& vqs);
        friend VQSTransform  operator*(Real real, const VQSTransform& vqs);

    public:
        Vector3    position;
        Real       scale = 1.0f;
        Quaternion rotation;
    };
}
