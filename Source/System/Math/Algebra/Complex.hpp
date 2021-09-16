
#pragma once
#include "../Utility/MathDef.hpp"

namespace CS460
{
    class Vector2;

    class Complex
    {
    public:
        Complex();
        explicit Complex(Real r, Real i);
        explicit Complex(const Vector2& vector);
        explicit Complex(Real radian);
        Complex(const Complex& rhs);
        ~Complex();

        void Set(Real r, Real i);
        void Set(const Vector2& vector);
        void Set(Real radian);
        void Set(const Complex& rhs);

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
        bool IsEqual(const Complex& rhs) const;
        bool IsNotEqual(const Complex& rhs) const;

        Vector2 ToVector() const;
        Real    ToRadian() const;

        Complex Conjugate() const;
        Complex Inverse() const;
        Real    DotProduct(const Complex& complex) const;
        Vector2 Rotate(const Vector2& vector) const;
        Complex Multiply(const Complex& rhs) const;
        void    AddRotation(const Complex& complex);
        void    AddRotation(Real radian);

    public:
        Complex  operator-() const;
        Complex& operator=(const Complex& rhs);
        bool     operator ==(const Complex& rhs) const;
        bool     operator !=(const Complex& rhs) const;
        Real     operator[](size_t i) const;
        Real&    operator[](size_t i);
        Complex  operator*(const Complex& rhs) const;
        Complex& operator *=(const Complex& rhs);
        Complex  operator+(const Complex& rhs) const;
        Complex& operator+=(const Complex& rhs);
        Complex  operator-(const Complex& rhs) const;
        Complex& operator-=(const Complex& rhs);
        Complex  operator*(Real scalar) const;
        Complex& operator*=(Real scalar);

    public:
        friend Complex operator*(Real scalar, const Complex& complex);

    public:
        Real r, i;
    };
}
