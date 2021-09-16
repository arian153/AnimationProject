
#pragma once
#include "../Utility/MathDef.hpp"
#include "../Structure/EulerAngle.hpp"

namespace CS460
{
    class AxisRadian;
    class Quaternion;
    class Vector3;
    class Vector4;
    class Matrix33;

    class Matrix44
    {
    public:
        explicit Matrix44(
            Real c00 = 1.0f, Real c01 = 0.0f, Real c02 = 0.0f, Real c03 = 0.0f,
            Real c04 = 0.0f, Real c05 = 1.0f, Real c06 = 0.0f, Real c07 = 0.0f,
            Real c08 = 0.0f, Real c09 = 0.0f, Real c10 = 1.0f, Real c11 = 0.0f,
            Real c12 = 0.0f, Real c13 = 0.0f, Real c14 = 0.0f, Real c15 = 1.0f);

        Matrix44(const Matrix44& rhs);
        ~Matrix44();

        void SetRows(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4);
        void SetColumns(const Vector4& col1, const Vector4& col2, const Vector4& col3, const Vector4& col4);
        void SetDiagonal(Real a = 1.0f, Real b = 1.0f, Real c = 1.0f, Real d = 1.0f);

        void AddVectorRow(size_t i, const Vector4& vector);
        void AddVectorRow(size_t i, const Vector3& vector, Real w = 0.0f);
        void AddVectorColumn(size_t i, const Vector4& vector);
        void AddVectorColumn(size_t i, const Vector3& vector, Real w = 0.0f);

        void SetClean();
        void SetIdentity();
        void SetInverse();
        void SetTranspose();
        void SetZero();

        void SetRotation(const Quaternion& quaternion);
        void SetRotation(const Matrix33& rotation_matrix);
        void SetRotation(const EulerAngle& euler_angle);
        void SetRotation(const AxisRadian& axis_radian);
        void SetRotation(const Vector3& axis, Real radian);

        void SetRotationX(Real radian);
        void SetRotationY(Real radian);
        void SetRotationZ(Real radian);

        void SetScale(const Vector4& scale);
        void SetScale(const Vector3& scale, Real w = 1.0f);
        void SetTranslation(const Vector3& translation);

        bool IsZero() const;
        bool IsIdentity() const;
        bool IsEqual(const Matrix44& rhs) const;
        bool IsNotEqual(const Matrix44& rhs) const;

        void    GetRows(Vector4& row1, Vector4& row2, Vector4& row3, Vector4& row4) const;
        Vector4 GetRow(size_t i) const;

        void    GetColumns(Vector4& col1, Vector4& col2, Vector4& col3, Vector4& col4) const;
        Vector4 GetColumn(size_t i) const;

        Matrix44 Adjoint() const;
        Real     Determinant() const;
        Real     Trace() const;
        Matrix44 Inverse() const;
        Matrix44 Transpose() const;
        Matrix44 HadamardProduct(const Matrix44& rhs) const;

        Vector3 TransformPoint(const Vector3& point);
        Vector3 TransformVector(const Vector3& vector);
        Vector3 TransformVectorRotatingOrigin(const Vector3& vector, const Vector3& origin);

    public:
        friend Matrix44 Inverse(const Matrix44& mat);
        friend Matrix44 Transpose(const Matrix44& mat);
        friend Matrix44 HadamardProduct(const Matrix44& mat1, const Matrix44& mat2);

    public:
        Matrix44&            operator=(const Matrix44& rhs);
        Real                 operator()(size_t i, size_t j) const;
        Real&                operator()(size_t i, size_t j);
        Real                 operator[](size_t i) const;
        Real&                operator[](size_t i);
        bool                 operator==(const Matrix44& rhs) const;
        bool                 operator!=(const Matrix44& rhs) const;
        Matrix44             operator+(const Matrix44& rhs) const;
        Matrix44&            operator+=(const Matrix44& rhs);
        Matrix44             operator-(const Matrix44& rhs) const;
        Matrix44&            operator-=(const Matrix44& rhs);
        Matrix44             operator-() const;
        Matrix44             operator*(const Matrix44& matrix) const;
        Matrix44&            operator*=(const Matrix44& matrix);
        Vector4              operator*(const Vector4& vector) const;
        friend Vector4       operator*(const Vector4& vector, const Matrix44& matrix);
        Matrix44             operator*(Real real) const;
        Matrix44&            operator*=(Real real);
        friend Matrix44      operator*(Real real, const Matrix44& matrix);
        friend std::ostream& operator<<(std::ostream& os, const Matrix44& rhs);

    public:
        static Matrix44 Identity();
        static Matrix44 Zero();
        static Real     Determinant33(Real c0, Real c1, Real c2, Real c3, Real c4, Real c5, Real c6, Real c7, Real c8);

    public:
        Real data[16];
    };
}
