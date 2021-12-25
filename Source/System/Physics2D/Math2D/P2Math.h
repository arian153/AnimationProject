#pragma once
#include <cmath>

namespace P2D
{
    using Real = float;
    constexpr Real EPSILON = 0.00001f;

    namespace Math
    {
        inline bool IsValid(Real rhs)
        {
            return !std::isnan(rhs) && std::isfinite(rhs);
        }

        inline bool IsInfinite(Real rhs)
        {
            return !std::isfinite(rhs);
        }

        inline bool IsZero(Real rhs)
        {
            return std::abs(rhs) < EPSILON;
        }

        inline bool IsEqual(Real a, Real b)
        {
            return (std::abs(a - b) < EPSILON);
        }

        inline Real ClearError(Real value, Real digit = EPSILON)
        {
            return std::round(value / digit) * digit;
        }
    }

    class Vector2
    {
    public:
        Vector2();
        ~Vector2();

        Vector2(Real x, Real y);

        void    Clear();
        void    Fix(Real digit);
        Vector2 Fixed(Real digit) const;
        void    Invert();
        Vector2 Inverted() const;
        void    Normalize();
        Vector2 Normalized() const;
        Vector2 Unit() const;

        Real Length() const;
        Real Dot(const Vector2& other) const;
        Real Cross(const Vector2& other) const;

        Vector2 Add(const Vector2& other) const;
        Vector2 Sub(const Vector2& other) const;
        Vector2 Div(Real other) const;
        Vector2 Mul(Real other) const;
        bool    Equal(const Vector2& other) const;
        Vector2 To(const Vector2& other) const;

        bool IsValid() const;
        void Validate();

    public:
        Real x = 0;
        Real y = 0;
    };

    class Orientation
    {
    public:
        Orientation();
        ~Orientation();

        explicit Orientation(Real angle);
        Orientation(Real sine, Real cosine);

        void Set(Real angle);
        void Set(Real sine, Real cosine);
        void SetIdentity();
        void SetInverse();

        void AddRotation(Real angle);
        void AddRotation(const Orientation& r);

        Real Angle() const;

        Vector2 XAxis() const;
        Vector2 YAxis() const;

        Orientation Inverse() const;
        Orientation Concatenate(const Orientation& r) const;
        Orientation ConcatenateInverse(const Orientation& r) const;

        static Orientation Concatenate(const Orientation& q, const Orientation& r);
        static Orientation ConcatenateInverse(const Orientation& q, const Orientation& r);

    public:
        Real s = 0;
        Real c = 1;
    };
}
