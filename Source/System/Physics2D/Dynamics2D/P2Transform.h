#pragma once
#include "../Math2D/P2Math.h"

namespace P2D
{
    class Transform
    {
    public:
        Transform();
        ~Transform();

        void SetIdentity();
        void Set(const Vector2& pos, Real angle);

        Vector2 ToWorldVector(const Vector2& p) const;
        Vector2 ToWorldPoint(const Vector2& p) const;
        Vector2 ToLocalVector(const Vector2& p) const;
        Vector2 ToLocalPoint(const Vector2& p) const;

        Transform Concatenate(const Transform& r) const;
        Transform ConcatenateInverse(const Transform& r) const;

    public:
        Vector2     position;
        Orientation rotation;
    };
}
