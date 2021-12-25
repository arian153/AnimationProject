#include "P2Math.h"

namespace P2D
{
    Vector2::Vector2()
    {
    }

    Vector2::~Vector2()
    {
    }

    Vector2::Vector2(Real _x, Real _y)
        : x(_x), y(_y)
    {
    }

    void Vector2::Clear()
    {
        x = 0;
        y = 0;
    }

    void Vector2::Fix(Real digit)
    {
        x = Math::ClearError(x, digit);
        y = Math::ClearError(y, digit);
    }

    Vector2 Vector2::Fixed(Real digit) const
    {
        return Vector2(Math::ClearError(x, digit), Math::ClearError(y, digit));
    }

    void Vector2::Invert()
    {
        x *= -1;
        y *= -1;
    }

    Vector2 Vector2::Inverted() const
    {
        return Vector2(x * -1, y * -1);
    }

    void Vector2::Normalize()
    {
        Real len = std::sqrt(x * x + y * y);
        if (Math::IsZero(len))
        {
            Real inv = 1 / len;
            x *= inv;
            y *= inv;
        }
    }

    Vector2 Vector2::Normalized() const
    {
        Real len = std::sqrt(x * x + y * y);
        if (Math::IsZero(len))
        {
            Real inv = 1 / len;
            return Vector2(x * inv, y * inv);
        }
        return Vector2(x, y);
    }

    Vector2 Vector2::Unit() const
    {
        Real len = std::sqrt(x * x + y * y);
        if (Math::IsZero(len))
        {
            Real inv = 1 / len;
            return Vector2(x * inv, y * inv);
        }
        return Vector2(0, 0);
    }

    Real Vector2::Length() const
    {
        return std::sqrt(x * x + y * y);
    }

    Real Vector2::Dot(const Vector2& v) const
    {
        return x * v.x + y * v.y;
    }

    Real Vector2::Cross(const Vector2& v) const
    {
        return x * v.y - y * v.x;
    }

    Vector2 Vector2::Add(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 Vector2::Sub(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 Vector2::Div(Real other) const
    {
        return Vector2(x / other, y / other);
    }

    Vector2 Vector2::Mul(Real other) const
    {
        return Vector2(x * other, y * other);
    }

    bool Vector2::Equal(const Vector2& other) const
    {
        return Math::IsEqual(x, other.x) && Math::IsEqual(y, other.y);
    }

    Vector2 Vector2::To(const Vector2& other) const
    {
        return Vector2(other.x - x, other.y - y);
    }

    bool Vector2::IsValid() const
    {
        return Math::IsValid(x) && Math::IsValid(y);
    }

    void Vector2::Validate()
    {
        if (!Math::IsValid(x))
            x = 0;
        if (!Math::IsValid(y))
            y = 0;
    }
}
