
#pragma once
#include "../Utility/MathDef.hpp"
#include "../Structure/EulerAngle.hpp"
#include "../Structure/AxisRadian.hpp"

namespace CS460
{
    class Quaternion;
    class Vector3;
    class Vector2;

    class Matrix33
    {
    public:
        explicit Matrix33(
            Real c0 = 1.0f, Real c1 = 0.0f, Real c2 = 0.0f,
            Real c3 = 0.0f, Real c4 = 1.0f, Real c5 = 0.0f,
            Real c6 = 0.0f, Real c7 = 0.0f, Real c8 = 1.0f);

        Matrix33(const Matrix33& rhs);
        explicit Matrix33(const Quaternion& quaternion);
        explicit Matrix33(const EulerAngle& euler_angle);
        explicit Matrix33(const AxisRadian& axis_radian);
        explicit Matrix33(const Vector3& axis, Real radian);
        explicit Matrix33(const Matrix44& matrix);

        ~Matrix33();

        void SetRows(const Vector3& row1, const Vector3& row2, const Vector3& row3);
        void SetColumns(const Vector3& col1, const Vector3& col2, const Vector3& col3);
        void SetDiagonal(Real a = 1.0f, Real b = 1.0f, Real c = 1.0f);

        void SetClean();
        void SetIdentity();
        void SetInverse();
        void SetTranspose();
        void SetZero();

        void SetRotation(const Quaternion& quaternion);
        void SetRotation(const EulerAngle& euler_angle);
        void SetRotation(const AxisRadian& axis_radian);
        void SetRotation(const Vector3& axis, Real radian);

        void SetRotationX(Real radian);
        void SetRotationY(Real radian);
        void SetRotationZ(Real radian);

        void SetScale(const Vector3& scale);
        void SetTranslation(const Vector2& translation);

        void SetSkew(const Vector3& v);

        bool IsZero() const;
        bool IsIdentity() const;
        bool IsEqual(const Matrix33& rhs) const;
        bool IsNotEqual(const Matrix33& rhs) const;

        void    GetRows(Vector3& row1, Vector3& row2, Vector3& row3) const;
        Vector3 GetRow(size_t i) const;

        void    GetColumns(Vector3& col1, Vector3& col2, Vector3& col3) const;
        Vector3 GetColumn(size_t i) const;

        Quaternion ToQuaternion() const;
        AxisRadian ToAxisRadian() const;
        EulerAngle ToEulerAngle() const;

        Matrix33 Adjoint() const;
        Real     Determinant() const;
        Real     Trace() const;
        Matrix33 Inverse() const;
        Matrix33 Transpose() const;
        Matrix33 HadamardProduct(const Matrix33& rhs) const;

    public:
        friend Matrix33 Inverse(const Matrix33& mat);
        friend Matrix33 Transpose(const Matrix33& mat);
        friend Matrix33 HadamardProduct(const Matrix33& mat1, const Matrix33& mat2);
        friend Matrix33 Skew(const Vector3& v);

    public:
        Matrix33&            operator=(const Matrix33& rhs);
        Matrix33&            operator=(const Matrix44& rhs);
        Real                 operator()(size_t i, size_t j) const;
        Real&                operator()(size_t i, size_t j);
        Real                 operator[](size_t i) const;
        Real&                operator[](size_t i);
        bool                 operator==(const Matrix33& rhs) const;
        bool                 operator!=(const Matrix33& rhs) const;
        Matrix33             operator+(const Matrix33& rhs) const;
        Matrix33&            operator+=(const Matrix33& rhs);
        Matrix33             operator-(const Matrix33& rhs) const;
        Matrix33&            operator-=(const Matrix33& rhs);
        Matrix33             operator-() const;
        Matrix33             operator*(const Matrix33& matrix) const;
        Matrix33&            operator*=(const Matrix33& matrix);
        Vector3              operator*(const Vector3& vector) const;
        friend Vector3       operator*(const Vector3& vector, const Matrix33& matrix);
        Matrix33             operator*(Real real) const;
        Matrix33&            operator*=(Real real);
        friend Matrix33      operator*(Real real, const Matrix33& matrix);
        friend std::ostream& operator<<(std::ostream& os, const Matrix33& rhs);

    public:
        static Matrix33 Identity();
        static Matrix33 Zero();

    public:
        Real data[9];
    };
}
