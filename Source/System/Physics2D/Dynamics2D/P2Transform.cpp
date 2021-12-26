#include "P2Transform.h"

namespace P2D
{
    Transform::Transform()
    {
    }

    Transform::~Transform()
    {
    }

    void Transform::SetIdentity()
    {
        position.Clear();
        rotation.SetIdentity();
    }

    void Transform::Set(const Vector2& pos, Real angle)
    {
        position = pos;
        rotation.Set(angle);
    }

    Vector2 Transform::ToWorldVector(const Vector2& p) const
    {
        return rotation.Rotate(p);
    }

    Vector2 Transform::ToWorldPoint(const Vector2& p) const
    {
        return rotation.Rotate(p).Add(position);
    }

    Vector2 Transform::ToLocalVector(const Vector2& p) const
    {
        return rotation.RotateInverse(p);
    }

    Vector2 Transform::ToLocalPoint(const Vector2& p) const
    {
        return rotation.RotateInverse(p.Sub(position));
    }

  
    Transform Transform::Concatenate(const Transform& r) const
    {
        Transform result;
        result.rotation = rotation.Concatenate(r.rotation);
        result.position = rotation.Rotate(r.position).Add(position);
        return result;
    }

    Transform Transform::ConcatenateInverse(const Transform& r) const
    {
        Transform result;
        result.rotation = rotation.ConcatenateInverse(r.rotation);
        result.position = rotation.RotateInverse(r.position.Sub(position));
        return result;
    }
}
