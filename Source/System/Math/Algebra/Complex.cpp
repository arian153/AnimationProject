
#include "Complex.hpp"
#include "Vector2.hpp"
#include "..//Utility/Utility.hpp"

namespace CS460
{
    Complex::Complex()
        : r(1.0f), i(0.0f)
    {
    }

    Complex::Complex(Real r, Real i)
        : r(r), i(i)
    {
    }

    Complex::Complex(const Vector2& vector)
    {
        Set(vector);
    }

    Complex::Complex(Real radian)
    {
        Set(radian);
    }

    Complex::Complex(const Complex& rhs)
        : r(rhs.r), i(rhs.i)
    {
    }

    Complex::~Complex()
    {
    }

    void Complex::Set(Real r, Real i)
    {
        this->r = r;
        this->i = i;
    }

    void Complex::Set(const Vector2& vector)
    {
        r = vector.x;
        i = vector.y;
    }

    void Complex::Set(Real radian)
    {
        r = cosf(radian);
        i = sinf(radian);
    }

    void Complex::Set(const Complex& rhs)
    {
        r = rhs.r;
        i = rhs.i;
    }

    void Complex::SetNormalize()
    {
        Real norm = r * r + i * i;
        if (Math::IsZero(norm) == true)
        {
            r = 1.0f;
            return;
        }
        Real multiplier = Math::InvSqrt(norm);
        r *= multiplier;
        i *= multiplier;
    }

    void Complex::SetClean()
    {
        if (Math::IsZero(r))
            r = 0.0f;
        if (Math::IsZero(i))
            i = 0.0f;
    }

    void Complex::SetZero()
    {
        r = i = 0.0f;
    }

    void Complex::SetIdentity()
    {
        r = 1.0f;
        i = 0.0f;
    }

    void Complex::SetConjugate()
    {
        i = -i;
    }

    void Complex::SetInverse()
    {
        Real norm = r * r + i * i;
        if (Math::IsZero(norm))
        {
            return;
        }
        Real inv_norm = 1.0f / norm;
        r             = inv_norm * r;
        i             = -inv_norm * i;
    }

    Real Complex::Norm() const
    {
        return sqrtf(r * r + i * i);
    }

    Real Complex::NormSquared() const
    {
        return r * r + i * i;
    }

    bool Complex::IsZero() const
    {
        return Math::IsZero(r * r + i * i);
    }

    bool Complex::IsUnit() const
    {
        return Math::IsZero(1.0f - (r * r + i * i));
    }

    bool Complex::IsIdentity() const
    {
        return Math::IsZero(1.0f - r) && Math::IsZero(i);
    }

    bool Complex::IsEqual(const Complex& rhs) const
    {
        if (Math::IsEqual(r, rhs.r) == false)
            return false;
        if (Math::IsEqual(i, rhs.i) == false)
            return false;
        return true;
    }

    bool Complex::IsNotEqual(const Complex& rhs) const
    {
        if (Math::IsEqual(r, rhs.r) == false)
            return true;
        if (Math::IsEqual(i, rhs.i) == false)
            return true;
        return false;
    }

    Vector2 Complex::ToVector() const
    {
        return Vector2(r, i);
    }

    Real Complex::ToRadian() const
    {
        return acosf(r);
    }

    Complex Complex::Conjugate() const
    {
        return Complex(r, -i);
    }

    Complex Complex::Inverse() const
    {
        Real norm = r * r + i * i;
        if (Math::IsZero(norm))
        {
            return Complex(r, i);
        }
        Real inv_norm = 1.0f / norm;
        return Complex(inv_norm * r, -inv_norm * i);
    }

    Real Complex::DotProduct(const Complex& complex) const
    {
        return r * complex.r + i * complex.i;
    }

    Vector2 Complex::Rotate(const Vector2& vector) const
    {
        Complex v(vector);
        Complex result = ((*this) * v);
        return result.ToVector();
    }

    Complex Complex::Multiply(const Complex& rhs) const
    {
        return Complex(r * rhs.r - i * rhs.i, r * rhs.i + rhs.r * i);
    }

    void Complex::AddRotation(const Complex& complex)
    {
        *this = complex * (*this);
        SetNormalize();
    }

    void Complex::AddRotation(Real radian)
    {
        Complex rot(radian);
        *this = rot * (*this);
        SetNormalize();
    }

    Complex Complex::operator-() const
    {
        return Complex(-r, -i);
    }

    Complex& Complex::operator=(const Complex& rhs)
    {
        if (this != &rhs)
        {
            r = rhs.r;
            i = rhs.i;
        }
        return *this;
    }

    bool Complex::operator==(const Complex& rhs) const
    {
        if (Math::IsEqual(r, rhs.r) == false)
            return false;
        if (Math::IsEqual(i, rhs.i) == false)
            return false;
        return true;
    }

    bool Complex::operator!=(const Complex& rhs) const
    {
        if (Math::IsEqual(r, rhs.r) == false)
            return true;
        if (Math::IsEqual(i, rhs.i) == false)
            return true;
        return false;
    }

    Real Complex::operator[](size_t _i) const
    {
        return (&r)[_i];
    }

    Real& Complex::operator[](size_t _i)
    {
        return (&r)[_i];
    }

    Complex Complex::operator*(const Complex& rhs) const
    {
        return Complex(r * rhs.r - i * rhs.i, r * rhs.i + rhs.r * i);
    }

    Complex& Complex::operator*=(const Complex& rhs)
    {
        auto c = *this;
        r      = c.r * rhs.r - c.i * rhs.i;
        i      = c.r * rhs.i + c.i * rhs.r;
        return *this;
    }

    Complex Complex::operator+(const Complex& rhs) const
    {
        return Complex(r + rhs.r, i + rhs.i);
    }

    Complex& Complex::operator+=(const Complex& rhs)
    {
        r += rhs.r;
        i += rhs.i;
        return *this;
    }

    Complex Complex::operator-(const Complex& rhs) const
    {
        return Complex(r - rhs.r, i - rhs.i);
    }

    Complex& Complex::operator-=(const Complex& rhs)
    {
        r -= rhs.r;
        i -= rhs.i;
        return *this;
    }

    Complex Complex::operator*(Real scalar) const
    {
        return Complex(r * scalar, i * scalar);
    }

    Complex& Complex::operator*=(Real scalar)
    {
        r *= scalar;
        i *= scalar;
        return *this;
    }

    Complex operator*(Real scalar, const Complex& complex)
    {
        return Complex(complex.r * scalar, complex.i * scalar);
    }
}
