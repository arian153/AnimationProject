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

    Orientation::Orientation()
    {
    }

    Orientation::~Orientation()
    {
    }

    Orientation::Orientation(Real angle)
    {
        s = std::sin(angle);
        c = std::cos(angle);
    }

    Orientation::Orientation(Real sine, Real cosine)
    {
        s = sine;
        c = cosine;
    }

    void Orientation::Set(Real angle)
    {
        s = std::sin(angle);
        c = std::cos(angle);
    }

    void Orientation::Set(Real sine, Real cosine)
    {
        s = sine;
        c = cosine;
    }

    void Orientation::SetIdentity()
    {
        s = 0;
        c = 1;
    }

    void Orientation::SetInverse()
    {
        s = -s;
    }

    void Orientation::AddRotation(Real angle)
    {
        Orientation result = Concatenate(Orientation(angle));

        s = result.s;
        c = result.c;
    }

    void Orientation::AddRotation(const Orientation& r)
    {
        Orientation result = Concatenate(r);

        s = result.s;
        c = result.c;
    }

    Real Orientation::Angle() const
    {
        return std::atan2(s, c);
    }

    Vector2 Orientation::XAxis() const
    {
        return Vector2(c, s);
    }

    Vector2 Orientation::YAxis() const
    {
        return Vector2(-s, c);
    }

    Orientation Orientation::Inverse() const
    {
        return Orientation(-s, c);
    }

    Orientation Orientation::Concatenate(const Orientation& r) const
    {
        return Orientation(s * r.c + c * r.s, c * r.c - s * r.s);
    }

    Orientation Orientation::ConcatenateInverse(const Orientation& r) const
    {
        return Orientation(c * r.s - s * r.c, c * r.c + s * r.s);
    }

    Orientation Orientation::Concatenate(const Orientation& q, const Orientation& r)
    {
        Orientation qr;
        qr.s = q.s * r.c + q.c * r.s;
        qr.c = q.c * r.c - q.s * r.s;
        return qr;
    }

    Orientation Orientation::ConcatenateInverse(const Orientation& q, const Orientation& r)
    {
        Orientation qr;
        qr.s = q.c * r.s - q.s * r.c;
        qr.c = q.c * r.c + q.s * r.s;
        return qr;
    }
}
