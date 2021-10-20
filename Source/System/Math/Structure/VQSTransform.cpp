#include "VQSTransform.hpp"
#include <ostream>

#include "../Utility/Interpolation.hpp"
#include "../Utility/Utility.hpp"
#include "../Utility/MatrixUtility.hpp"

namespace CS460
{
    VQSTransform::VQSTransform()
    {
    }

    VQSTransform::VQSTransform(const VQSTransform& vqs)
        : position(vqs.position), scale(vqs.scale), rotation(vqs.rotation)
    {
    }

    VQSTransform::VQSTransform(const Vector3& pos, const Quaternion& rot, Real s)
        : position(pos), scale(s), rotation(rot)
    {
    }

    VQSTransform::~VQSTransform()
    {
    }

    void VQSTransform::Set(const Vector3& pos, const Quaternion& rot, Real s)
    {
        position = pos;
        rotation = rot;
        scale    = s;
    }

    Matrix44 VQSTransform::ToMatrix() const
    {
        Matrix44 result = Math::Matrix44::Scale(scale, scale, scale, 1.0f);
        result          = Math::Matrix44::Rotation(rotation) * result;
        result.AddVectorColumn(3, position);
        return result;
    }

    VQSTransform VQSTransform::Identity()
    {
        return VQSTransform(Vector3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), 1.0f);
    }

    VQSTransform VQSTransform::Inverse() const
    {
        Real       inv_scale = Math::IsZero(scale) ? 0.0f : 1.0f / scale;
        Quaternion inv_rot   = rotation.Inverse();
        return VQSTransform(inv_rot.Rotate(inv_scale * (-position)), inv_rot, inv_scale);
    }

    Vector3 VQSTransform::ApplyTransform(const Vector3& point) const
    {
        return rotation.Rotate(scale * point) + position;
    }

    Vector3 VQSTransform::ApplyInverse(const Vector3& point) const
    {
        Real       inv_scale = Math::IsZero(scale) ? 0.0f : 1.0f / scale;
        Quaternion inv_rot   = rotation.Inverse();
        Vector3    inv_pos   = inv_rot.Rotate(inv_scale * (-position));

        return inv_rot.Rotate(inv_scale * point) + inv_pos;
    }

    VQSTransform& VQSTransform::operator=(const VQSTransform& vqs)
    {
        if (this != &vqs)
        {
            position = vqs.position;
            scale    = vqs.scale;
            rotation = vqs.rotation;
        }
        return *this;
    }

    VQSTransform VQSTransform::operator+(const VQSTransform& vqs) const
    {
        return VQSTransform(position + vqs.position, rotation + vqs.rotation, scale + vqs.scale);
    }

    VQSTransform VQSTransform::operator*(Real real) const
    {
        return VQSTransform(position * real, rotation * real, scale * real);
    }

    VQSTransform VQSTransform::operator*(const VQSTransform& vqs) const
    {
        return VQSTransform(rotation.Rotate(scale * vqs.position) + position, rotation * vqs.rotation, scale * vqs.scale);
    }

    Vector3 VQSTransform::operator*(const Vector3& rhs) const
    {
        return rotation.Rotate(scale * rhs) + position;
    }

    VQSTransform Concatenate(const VQSTransform& a, const VQSTransform& b)
    {
        return VQSTransform(a.rotation.Rotate(a.scale * b.position) + a.position, a.rotation * b.rotation, a.scale * b.scale);
    }

    VQSTransform Interpolation(const VQSTransform& start, const VQSTransform& end, Real t)
    {
        return VQSTransform(
                            Lerp(start.position, end.position, t),
                            Slerp(start.rotation, end.rotation, t),
                            Elerp(start.scale, end.scale, t));
    }

    Matrix44 Concatenate(const Matrix44& a, const Matrix44& b)
    {
        return a * b;
    }

    std::ostream& operator<<(std::ostream& os, const VQSTransform& vqs)
    {
        os << "[" << vqs.position << ", " << vqs.rotation << ", " << vqs.scale << "]";
        return os;
    }

    VQSTransform operator*(Real real, const VQSTransform& vqs)
    {
        return VQSTransform(vqs.position * real, vqs.rotation * real, vqs.scale * real);
    }
}
