
#pragma once
#include "../Utility/MathDef.hpp"

namespace CS460
{
    class Vector2;

    class Matrix22
    {
    public:
        explicit Matrix22(
            Real c0 = 1.0f, Real c1 = 0.0f,
            Real c2 = 0.0f, Real c3 = 1.0f
        );
        Matrix22(const Matrix22& rhs);
        ~Matrix22();

        void SetRows(const Vector2& row1, const Vector2& row2);
        void SetColumns(const Vector2& col1, const Vector2& col2);
        void SetDiagonal(Real a = 1.0f, Real b = 1.0f);

        void SetClean();
        void SetIdentity();
        void SetInverse();
        void SetTranspose();
        void SetZero();

        bool IsZero() const;
        bool IsIdentity() const;
        bool IsEqual(const Matrix22& rhs) const;
        bool IsNotEqual(const Matrix22& rhs) const;

        void    GetRows(Vector2& row1, Vector2& row2) const;
        Vector2 GetRow(size_t i) const;

        void    GetColumns(Vector2& col1, Vector2& col2) const;
        Vector2 GetColumn(size_t i) const;

        Matrix22 Adjoint() const;
        Real     Determinant() const;
        Real     Trace() const;
        Matrix22 Inverse() const;
        Matrix22 Transpose() const;
        Matrix22 HadamardProduct(const Matrix22& rhs) const;

    public:
        friend Matrix22 Inverse(const Matrix22& mat);
        friend Matrix22 Transpose(const Matrix22& mat);
        friend Matrix22 HadamardProduct(const Matrix22& mat1, const Matrix22& mat2);

    public:
        Matrix22&            operator=(const Matrix22& rhs);
        Real                 operator()(size_t i, size_t j) const;
        Real&                operator()(size_t i, size_t j);
        Real                 operator[](size_t i) const;
        Real&                operator[](size_t i);
        bool                 operator==(const Matrix22& rhs) const;
        bool                 operator!=(const Matrix22& rhs) const;
        Matrix22             operator+(const Matrix22& rhs) const;
        Matrix22&            operator+=(const Matrix22& rhs);
        Matrix22             operator-(const Matrix22& rhs) const;
        Matrix22&            operator-=(const Matrix22& rhs);
        Matrix22             operator-() const;
        Matrix22             operator*(const Matrix22& matrix) const;
        Matrix22&            operator*=(const Matrix22& matrix);
        Vector2              operator*(const Vector2& vector) const;
        friend Vector2       operator*(const Vector2& vector, const Matrix22& matrix);
        Matrix22             operator*(Real real) const;
        Matrix22&            operator*=(Real real);
        friend Matrix22      operator*(Real real, const Matrix22& matrix);
        friend std::ostream& operator<<(std::ostream& os, const Matrix22& rhs);

    public:
        static Matrix22 Identity();
        static Matrix22 Zero();

    public:
        Real data[4];
    };
}
