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

    Matrix44 VQSTransform::ToMatrix() const
    {
        Matrix44 scale_mat = Math::Matrix44::Scale(Vector3(scale, scale, scale), 1.0f);
        Matrix44 rot_mat   = Math::Matrix44::Rotation(rotation);
        Matrix44 trans_mat = Math::Matrix44::Translation(position);

        //E5_TODO : need to test is this order is correct.
        return (trans_mat * (rot_mat * scale_mat));
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

    VQSTransform Multiply(const VQSTransform& a, const VQSTransform& b)
    {
        return VQSTransform(a.rotation.Rotate(a.scale * b.position) + a.position, a.rotation * b.rotation, a.scale * b.scale);
    }

    VQSTransform Interpolation(const VQSTransform& a, const VQSTransform& b, Real t)
    {
        return VQSTransform(
                            Lerp(a.position, b.position, t),
                            Slerp(a.rotation, b.rotation, t),
                            Elerp(a.scale, b.scale, t));
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
