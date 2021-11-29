#pragma once
#include "../Utility/MathDef.hpp"
#include "../Structure/EulerAngle.hpp"
#include "../Structure/AxisRadian.hpp"

namespace CS460
{
    class Vector3;
    class Matrix33;

    class Quaternion
    {
    public:
        explicit Quaternion(Real r = 1.0f, Real i = 0.0f, Real j = 0.0f, Real k = 0.0f);
        explicit Quaternion(const Vector3& vector);
        explicit Quaternion(const Matrix33& rotation_matrix);
        explicit Quaternion(const AxisRadian& axis_radian);
        explicit Quaternion(const EulerAngle& euler_angle);
        Quaternion(const Vector3& axis, Real radian);
        Quaternion(const Vector3& from, const Vector3& to);
        Quaternion(const Quaternion& rhs);

        ~Quaternion();

        void Set(Real r = 1.0f, Real i = 0.0f, Real j = 0.0f, Real k = 0.0f);
        void Set(const Vector3& from, const Vector3& to);
        void Set(const Vector3& axis, Real radian);
        void Set(const Vector3& vector);
        void Set(const Matrix33& rotation_matrix);
        void Set(const AxisRadian& axis_radian);
        void Set(const EulerAngle& euler_angle);
        void Set(const Quaternion& rhs);

        void SetNormalize();
        void SetClean();
        void SetZero();
        void SetIdentity();
        void SetConjugate();
        void SetInverse();

        Real Norm() const;
        Real NormSquared() const;

        bool IsZero() const;
        bool IsUnit() const;
        bool IsIdentity() const;
        bool IsEqual(const Quaternion& rhs) const;
        bool IsNotEqual(const Quaternion& rhs) const;
        bool IsLostAxis() const;

        Vector3    ToVector() const;
        Matrix33   ToMatrix() const;
        AxisRadian ToAxisRadian() const;
        EulerAngle ToEulerAngle() const;

        Quaternion Conjugate() const;
        Quaternion Inverse() const;
        Quaternion Unit() const;
        Real       DotProduct(const Quaternion& quaternion) const;
        Vector3    Rotate(const Vector3& vector) const;
        Vector3    RotateVector(const Vector3& vector) const;

        Quaternion Multiply(const Quaternion& rhs) const;
        void       AddRotation(const Vector3& axis, Real radian);
        void       AddRotation(const Quaternion& quaternion);
        void       AddRadian(Real radian);
        void       ChangeAxis(const Vector3& axis);

    public:
        friend Quaternion Conjugate(const Quaternion& quaternion);
        friend Quaternion Inverse(const Quaternion& quaternion);
        friend Real       DotProduct(const Quaternion& quat1, const Quaternion& quat2);
        friend Quaternion LinearInterpolation(const Quaternion& start, const Quaternion& end, Real t);
        friend Quaternion SphericalLinearInterpolation(const Quaternion& start, const Quaternion& end, Real t);
        friend Quaternion SwingTwistInterpolation(const Quaternion& start, const Quaternion& end, const Vector3& twist_axis, Real t);

    public:
        Quaternion                 operator-() const;
        Quaternion&                operator=(const Quaternion& rhs);
        bool                       operator ==(const Quaternion& rhs) const;
        bool                       operator !=(const Quaternion& rhs) const;
        Real                       operator[](size_t i) const;
        Real&                      operator[](size_t i);
        Quaternion                 operator*(const Quaternion& rhs) const;
        Quaternion&                operator *=(const Quaternion& rhs);
        Quaternion                 operator+(const Quaternion& rhs) const;
        Quaternion&                operator+=(const Quaternion& rhs);
        Quaternion                 operator-(const Quaternion& rhs) const;
        Quaternion&                operator-=(const Quaternion& rhs);
        Quaternion                 operator*(Real scalar) const;
        friend Quaternion          operator*(Real scalar, const Quaternion& quaternion);
        Quaternion&                operator*=(Real scalar);
        friend std::ostream&       operator<<(std::ostream& os, const Quaternion& rhs);
        friend std::wstringstream& operator<<(std::wstringstream& os, const Quaternion& rhs);
        Quaternion                 operator*(const Vector3& vector) const;
        friend Quaternion          operator*(const Vector3& vector, const Quaternion& quaternion);
        Matrix33                   operator*(const Matrix33& matrix) const;
        friend Matrix33            operator*(const Matrix33& matrix, const Quaternion& quaternion);

    public:
        static Quaternion Identity();

    public:
        Real r, i, j, k;
    };
}
