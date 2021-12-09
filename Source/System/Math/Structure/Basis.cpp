#include "Basis.hpp"

#include "../Algebra/Matrix33.hpp"
#include "../Algebra/Quaternion.hpp"

namespace CS460
{
    Basis::Basis()
    {
    }

    Basis::Basis(const Vector3& a, const Vector3& b, const Vector3& c)
        : i(a), j(b), k(c)
    {
        i.SetNormalize();
        j.SetNormalize();
        k.SetNormalize();
    }

    Basis::Basis(const Basis& rhs)
        : i(rhs.i), j(rhs.j), k(rhs.k)
    {
        i.SetNormalize();
        j.SetNormalize();
        k.SetNormalize();
    }

    Basis& Basis::operator=(const Basis& rhs)
    {
        if (this != &rhs)
        {
            this->i = rhs.i;
            this->j = rhs.j;
            this->k = rhs.k;
        }
        return *this;
    }

    Basis::~Basis()
    {
    }

    void Basis::CalculateBasis(const Vector3& normal)
    {
        // find least axis of least significant component
        Real    abs_x = fabsf(normal.x);
        Real    abs_y = fabsf(normal.y);
        Real    abs_z = fabsf(normal.z);
        Vector3 axis(0.0f, 0.0f, 0.0f);
        if (abs_x > abs_y)
        {
            if (abs_x > abs_z)
                axis.x = 1.0f; // X > Y > Z, X > Z > Y
            else
                axis.z = 1.0f; // Z > X > Y
        }
        else
        {
            if (abs_y > abs_z)
                axis.y = 1.0f; // Y > X > Z, Y > Z > X
            else
                axis.z = 1.0f; // Z > Y > X
        }
        // compute tangents
        i = normal.Normalize();
        j = normal.CrossProduct(axis).Normalize();
        k = i.CrossProduct(j).Normalize();
    }

    void Basis::CalculateBasisApprox(const Vector3& normal)
    {
        //sqrt(1/3) = 0.57735
        if (fabsf(normal.x) >= 0.57735f)
        {
            j.Set(normal.y, -normal.x, 0.0f);
        }
        else
        {
            j.Set(0.0f, normal.z, -normal.y);
        }
        i = normal.Normalize();
        j.SetNormalize();
        k = i.CrossProduct(j).Normalize();
    }

    void Basis::CalculateBasisQuaternion(const Vector3& normal)
    {
        i = normal.Normalize();
        Quaternion rotation(i, Math::Vector3::Y_AXIS);
        j = rotation.Rotate(Math::Vector3::X_AXIS);
        j.SetNormalize();
        k = rotation.Rotate(Math::Vector3::Z_AXIS);
        k.SetNormalize();
    }

    void Basis::CalculateBasis(const Vector3& a, const Vector3& b)
    {
        i = a.Normalize();
        j = b.Normalize();
        k = a.CrossProduct(b).Normalize();
    }

    void Basis::Rotate(const Quaternion& orientation)
    {
        i = orientation.Rotate(i);
        j = orientation.Rotate(j);
        k = orientation.Rotate(k);
    }

    void Basis::Normalize()
    {
        i.SetNormalize();
        j.SetNormalize();
        k.SetNormalize();
    }

    Vector3 Basis::ApplyTransform(const Vector3& vector) const
    {
        Matrix33 matrix;
        matrix.SetColumns(i, j, k);
        return matrix.Inverse() * vector;
    }
}
