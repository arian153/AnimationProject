#include "Matrix44.hpp"
#include "Vector4.hpp"
#include "Vector3.hpp"
#include "Matrix33.hpp"
#include "Quaternion.hpp"
#include "../Utility/Utility.hpp"
#include <ostream>
#include "../../Core/Utility/CoreUtility.hpp"

namespace CS460
{
    Matrix44::Matrix44(
        Real c00, Real c01, Real c02, Real c03,
        Real c04, Real c05, Real c06, Real c07,
        Real c08, Real c09, Real c10, Real c11,
        Real c12, Real c13, Real c14, Real c15)
    {
        data[0]  = c00;
        data[1]  = c01;
        data[2]  = c02;
        data[3]  = c03;
        data[4]  = c04;
        data[5]  = c05;
        data[6]  = c06;
        data[7]  = c07;
        data[8]  = c08;
        data[9]  = c09;
        data[10] = c10;
        data[11] = c11;
        data[12] = c12;
        data[13] = c13;
        data[14] = c14;
        data[15] = c15;
    }

    Matrix44::Matrix44(const Matrix44& rhs)
    {
        data[0]  = rhs.data[0];
        data[1]  = rhs.data[1];
        data[2]  = rhs.data[2];
        data[3]  = rhs.data[3];
        data[4]  = rhs.data[4];
        data[5]  = rhs.data[5];
        data[6]  = rhs.data[6];
        data[7]  = rhs.data[7];
        data[8]  = rhs.data[8];
        data[9]  = rhs.data[9];
        data[10] = rhs.data[10];
        data[11] = rhs.data[11];
        data[12] = rhs.data[12];
        data[13] = rhs.data[13];
        data[14] = rhs.data[14];
        data[15] = rhs.data[15];
    }

    Matrix44::~Matrix44()
    {
    }

    void Matrix44::SetRows(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4)
    {
        data[0]  = row1.x;
        data[1]  = row1.y;
        data[2]  = row1.z;
        data[3]  = row1.w;
        data[4]  = row2.x;
        data[5]  = row2.y;
        data[6]  = row2.z;
        data[7]  = row2.w;
        data[8]  = row3.x;
        data[9]  = row3.y;
        data[10] = row3.z;
        data[11] = row3.w;
        data[12] = row4.x;
        data[13] = row4.y;
        data[14] = row4.z;
        data[15] = row4.w;
    }

    void Matrix44::SetColumns(const Vector4& col1, const Vector4& col2, const Vector4& col3, const Vector4& col4)
    {
        data[0]  = col1.x;
        data[1]  = col2.x;
        data[2]  = col3.x;
        data[3]  = col4.x;
        data[4]  = col1.y;
        data[5]  = col2.y;
        data[6]  = col3.y;
        data[7]  = col4.y;
        data[8]  = col1.z;
        data[9]  = col2.z;
        data[10] = col3.z;
        data[11] = col4.z;
        data[12] = col1.w;
        data[13] = col2.w;
        data[14] = col3.w;
        data[15] = col4.w;
    }

    void Matrix44::SetDiagonal(Real a, Real b, Real c, Real d)
    {
        data[0]  = a;
        data[5]  = b;
        data[10] = c;
        data[15] = d;
    }

    void Matrix44::AddVectorRow(size_t i, const Vector4& vector)
    {
        size_t index = i % 4;
        data[4 * index] += vector.x;
        data[4 * index + 1] += vector.y;
        data[4 * index + 2] += vector.z;
        data[4 * index + 3] += vector.w;
    }

    void Matrix44::AddVectorRow(size_t i, const Vector3& vector, Real w)
    {
        size_t index = i % 4;
        data[4 * index] += vector.x;
        data[4 * index + 1] += vector.y;
        data[4 * index + 2] += vector.z;
        data[4 * index + 3] += w;
    }

    void Matrix44::AddVectorColumn(size_t i, const Vector4& vector)
    {
        size_t index = i % 4;
        data[index] += vector.x;
        data[index + 4] += vector.y;
        data[index + 8] += vector.z;
        data[index + 12] += vector.w;
    }

    void Matrix44::AddVectorColumn(size_t i, const Vector3& vector, Real w)
    {
        size_t index = i % 4;
        data[index] += vector.x;
        data[index + 4] += vector.y;
        data[index + 8] += vector.z;
        data[index + 12] += w;
    }

    void Matrix44::SetClean()
    {
        for (size_t i = 0; i < 16; ++i)
        {
            if (Math::IsZero(data[i]))
                data[i] = 0.0f;
        }
    }

    void Matrix44::SetIdentity()
    {
        data[0]  = 1.0f;
        data[1]  = 0.0f;
        data[2]  = 0.0f;
        data[3]  = 0.0f;
        data[4]  = 0.0f;
        data[5]  = 1.0f;
        data[6]  = 0.0f;
        data[7]  = 0.0f;
        data[8]  = 0.0f;
        data[9]  = 0.0f;
        data[10] = 1.0f;
        data[11] = 0.0f;
        data[12] = 0.0f;
        data[13] = 0.0f;
        data[14] = 0.0f;
        data[15] = 1.0f;
    }

    void Matrix44::SetInverse()
    {
        *this = this->Inverse();
    }

    void Matrix44::SetTranspose()
    {
        *this = this->Transpose();
    }

    void Matrix44::SetZero()
    {
        data[0]  = 0.0f;
        data[1]  = 0.0f;
        data[2]  = 0.0f;
        data[3]  = 0.0f;
        data[4]  = 0.0f;
        data[5]  = 0.0f;
        data[6]  = 0.0f;
        data[7]  = 0.0f;
        data[8]  = 0.0f;
        data[9]  = 0.0f;
        data[10] = 0.0f;
        data[11] = 0.0f;
        data[12] = 0.0f;
        data[13] = 0.0f;
        data[14] = 0.0f;
        data[15] = 0.0f;
    }

    void Matrix44::SetRotation(const Quaternion& quaternion)
    {
        if (quaternion.IsUnit())
        {
            Real xs  = quaternion.i + quaternion.i;
            Real ys  = quaternion.j + quaternion.j;
            Real zs  = quaternion.k + quaternion.k;
            Real wx  = quaternion.r * xs;
            Real wy  = quaternion.r * ys;
            Real wz  = quaternion.r * zs;
            Real xx  = quaternion.i * xs;
            Real xy  = quaternion.i * ys;
            Real xz  = quaternion.i * zs;
            Real yy  = quaternion.j * ys;
            Real yz  = quaternion.j * zs;
            Real zz  = quaternion.k * zs;
            data[0]  = 1.0f - (yy + zz);
            data[1]  = xy - wz;
            data[2]  = xz + wy;
            data[3]  = 0.0f;
            data[4]  = xy + wz;
            data[5]  = 1.0f - (xx + zz);
            data[6]  = yz - wx;
            data[7]  = 0.0f;
            data[8]  = xz - wy;
            data[9]  = yz + wx;
            data[10] = 1.0f - (xx + yy);
            data[11] = 0.0f;
            data[12] = 0.0f;
            data[13] = 0.0f;
            data[14] = 0.0f;
            data[15] = 1.0f;
        }
    }

    void Matrix44::SetRotation(const Matrix33& rotation_matrix)
    {
        data[0]  = rotation_matrix.data[0];
        data[1]  = rotation_matrix.data[1];
        data[2]  = rotation_matrix.data[2];
        data[3]  = 0.0f;
        data[4]  = rotation_matrix.data[3];
        data[5]  = rotation_matrix.data[4];
        data[6]  = rotation_matrix.data[5];
        data[7]  = 0.0f;
        data[8]  = rotation_matrix.data[6];
        data[9]  = rotation_matrix.data[7];
        data[10] = rotation_matrix.data[8];
        data[11] = 0.0f;
        data[12] = 0.0f;
        data[13] = 0.0f;
        data[14] = 0.0f;
        data[15] = 1.0f;
    }

    void Matrix44::SetRotation(const EulerAngle& euler_angle)
    {
        Real sin_x = sinf(euler_angle.x_rotation);
        Real cos_x = cosf(euler_angle.x_rotation);
        Real sin_y = sinf(euler_angle.y_rotation);
        Real cos_y = cosf(euler_angle.y_rotation);
        Real sin_z = sinf(euler_angle.z_rotation);
        Real cos_z = cosf(euler_angle.z_rotation);
        data[0]    = (cos_y * cos_z);
        data[1]    = -(cos_y * sin_z);
        data[2]    = sin_y;
        data[3]    = 0.0f;
        data[4]    = (sin_x * sin_y * cos_z) + (cos_x * sin_z);
        data[5]    = -(sin_x * sin_y * sin_z) + (cos_x * cos_z);
        data[6]    = -(sin_x * cos_y);
        data[7]    = 0.0f;
        data[8]    = -(cos_x * sin_y * cos_z) + (sin_x * sin_z);
        data[9]    = (cos_x * sin_y * sin_z) + (sin_x * cos_z);
        data[10]   = (cos_x * cos_y);
        data[11]   = 0.0f;
        data[12]   = 0.0f;
        data[13]   = 0.0f;
        data[14]   = 0.0f;
        data[15]   = 1.0f;
    }

    void Matrix44::SetRotation(const AxisRadian& axis_radian)
    {
        Real    sin       = sinf(axis_radian.radian);
        Real    cos       = cosf(axis_radian.radian);
        Real    t         = 1.0f - cos;
        Vector3 unit_axis = axis_radian.axis.Unit();
        Real    tx        = t * unit_axis.x;
        Real    ty        = t * unit_axis.y;
        Real    tz        = t * unit_axis.z;
        Real    sx        = sin * unit_axis.x;
        Real    sy        = sin * unit_axis.y;
        Real    sz        = sin * unit_axis.z;
        Real    txy       = tx * unit_axis.y;
        Real    tyz       = tx * unit_axis.z;
        Real    txz       = tx * unit_axis.z;
        data[0]           = (tx * unit_axis.x + cos);
        data[1]           = (txy - sz);
        data[2]           = (txz + sy);
        data[3]           = 0.0f;
        data[4]           = (txy + sz);
        data[5]           = (ty * unit_axis.y + cos);
        data[6]           = (tyz - sx);
        data[7]           = 0.0f;
        data[8]           = (txz - sy);
        data[9]           = (tyz + sx);
        data[10]          = (tz * unit_axis.z + cos);
        data[11]          = 0.0f;
        data[12]          = 0.0f;
        data[13]          = 0.0f;
        data[14]          = 0.0f;
        data[15]          = 1.0f;
    }

    void Matrix44::SetRotation(const Vector3& axis, Real radian)
    {
        Real    sin       = sinf(radian);
        Real    cos       = cosf(radian);
        Real    t         = 1.0f - cos;
        Vector3 unit_axis = axis.Unit();
        Real    tx        = t * unit_axis.x;
        Real    ty        = t * unit_axis.y;
        Real    tz        = t * unit_axis.z;
        Real    sx        = sin * unit_axis.x;
        Real    sy        = sin * unit_axis.y;
        Real    sz        = sin * unit_axis.z;
        Real    txy       = tx * unit_axis.y;
        Real    tyz       = tx * unit_axis.z;
        Real    txz       = tx * unit_axis.z;
        data[0]           = (tx * unit_axis.x + cos);
        data[1]           = (txy - sz);
        data[2]           = (txz + sy);
        data[3]           = 0.0f;
        data[4]           = (txy + sz);
        data[5]           = (ty * unit_axis.y + cos);
        data[6]           = (tyz - sx);
        data[7]           = 0.0f;
        data[8]           = (txz - sy);
        data[9]           = (tyz + sx);
        data[10]          = (tz * unit_axis.z + cos);
        data[11]          = 0.0f;
        data[12]          = 0.0f;
        data[13]          = 0.0f;
        data[14]          = 0.0f;
        data[15]          = 1.0f;
    }

    void Matrix44::SetRotationX(Real radian)
    {
        Real sin = sinf(radian);
        Real cos = cosf(radian);
        data[0]  = 1.0f;
        data[1]  = 0.0f;
        data[2]  = 0.0f;
        data[3]  = 0.0f;
        data[4]  = 0.0f;
        data[5]  = cos;
        data[6]  = -sin;
        data[7]  = 0.0f;
        data[8]  = 0.0f;
        data[9]  = sin;
        data[10] = cos;
        data[11] = 0.0f;
        data[12] = 0.0f;
        data[13] = 0.0f;
        data[14] = 0.0f;
        data[15] = 1.0f;
    }

    void Matrix44::SetRotationY(Real radian)
    {
        Real sin = sinf(radian);
        Real cos = cosf(radian);
        data[0]  = cos;
        data[1]  = 0.0f;
        data[2]  = sin;
        data[3]  = 0.0f;
        data[4]  = 0.0f;
        data[5]  = 1.0f;
        data[6]  = 0.0f;
        data[7]  = 0.0f;
        data[8]  = -sin;
        data[9]  = 0.0f;
        data[10] = cos;
        data[11] = 0.0f;
        data[12] = 0.0f;
        data[13] = 0.0f;
        data[14] = 0.0f;
        data[15] = 1.0f;
    }

    void Matrix44::SetRotationZ(Real radian)
    {
        Real sin = sinf(radian);
        Real cos = cosf(radian);
        data[0]  = cos;
        data[1]  = -sin;
        data[2]  = 0.0f;
        data[3]  = 0.0f;
        data[4]  = sin;
        data[5]  = cos;
        data[6]  = 0.0f;
        data[7]  = 0.0f;
        data[8]  = 0.0f;
        data[9]  = 0.0f;
        data[10] = 1.0f;
        data[11] = 0.0f;
        data[12] = 0.0f;
        data[13] = 0.0f;
        data[14] = 0.0f;
        data[15] = 1.0f;
    }

    void Matrix44::SetScale(const Vector4& scale)
    {
        data[0]  = scale.x;
        data[1]  = 0.0f;
        data[2]  = 0.0f;
        data[3]  = 0.0f;
        data[4]  = 0.0f;
        data[5]  = scale.y;
        data[6]  = 0.0f;
        data[7]  = 0.0f;
        data[8]  = 0.0f;
        data[9]  = 0.0f;
        data[10] = scale.z;
        data[11] = 0.0f;
        data[12] = 0.0f;
        data[13] = 0.0f;
        data[14] = 0.0f;
        data[15] = scale.w;
    }

    void Matrix44::SetScale(const Vector3& scale, Real w)
    {
        data[0]  = scale.x;
        data[1]  = 0.0f;
        data[2]  = 0.0f;
        data[3]  = 0.0f;
        data[4]  = 0.0f;
        data[5]  = scale.y;
        data[6]  = 0.0f;
        data[7]  = 0.0f;
        data[8]  = 0.0f;
        data[9]  = 0.0f;
        data[10] = scale.z;
        data[11] = 0.0f;
        data[12] = 0.0f;
        data[13] = 0.0f;
        data[14] = 0.0f;
        data[15] = w;
    }

    void Matrix44::SetTranslation(const Vector3& translation)
    {
        data[0]  = 1.0f;
        data[1]  = 0.0f;
        data[2]  = 0.0f;
        data[3]  = translation.x;
        data[4]  = 0.0f;
        data[5]  = 1.0f;
        data[6]  = 0.0f;
        data[7]  = translation.y;
        data[8]  = 0.0f;
        data[9]  = 0.0f;
        data[10] = 1.0f;
        data[11] = translation.z;
        data[12] = 0.0f;
        data[13] = 0.0f;
        data[14] = 0.0f;
        data[15] = 1.0f;
    }

    bool Matrix44::IsZero() const
    {
        for (size_t i = 0; i < 16; ++i)
        {
            if (Math::IsZero(data[i]) == false)
                return false;
        }
        return true;
    }

    bool Matrix44::IsIdentity() const
    {
        return Math::IsEqual(1.0f, data[0])
                && Math::IsEqual(1.0f, data[5])
                && Math::IsEqual(1.0f, data[10])
                && Math::IsEqual(1.0f, data[15])
                && Math::IsZero(data[1])
                && Math::IsZero(data[2])
                && Math::IsZero(data[3])
                && Math::IsZero(data[4])
                && Math::IsZero(data[6])
                && Math::IsZero(data[7])
                && Math::IsZero(data[8])
                && Math::IsZero(data[9])
                && Math::IsZero(data[11])
                && Math::IsZero(data[12])
                && Math::IsZero(data[13])
                && Math::IsZero(data[14]);
    }

    bool Matrix44::IsEqual(const Matrix44& rhs) const
    {
        for (size_t i = 0; i < 16; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return false;
        }
        return true;
    }

    bool Matrix44::IsNotEqual(const Matrix44& rhs) const
    {
        for (size_t i = 0; i < 16; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return true;
        }
        return false;
    }

    void Matrix44::GetRows(Vector4& row1, Vector4& row2, Vector4& row3, Vector4& row4) const
    {
        row1.x = data[0];
        row1.y = data[1];
        row1.z = data[2];
        row1.w = data[3];
        row2.x = data[4];
        row2.y = data[5];
        row2.z = data[6];
        row2.w = data[7];
        row3.x = data[8];
        row3.y = data[9];
        row3.z = data[10];
        row3.w = data[11];
        row4.x = data[12];
        row4.y = data[13];
        row4.z = data[14];
        row4.w = data[15];
    }

    Vector4 Matrix44::GetRow(size_t i) const
    {
        size_t val = i % 4;
        return Vector4(data[3 * val], data[3 * val + 1], data[3 * val + 2], data[3 * val + 3]);
    }

    void Matrix44::GetColumns(Vector4& col1, Vector4& col2, Vector4& col3, Vector4& col4) const
    {
        col1.x = data[0];
        col2.x = data[1];
        col3.x = data[2];
        col4.x = data[3];
        col1.y = data[4];
        col2.y = data[5];
        col3.y = data[6];
        col4.y = data[7];
        col1.z = data[8];
        col2.z = data[9];
        col3.z = data[10];
        col4.z = data[11];
        col1.w = data[12];
        col2.w = data[13];
        col3.w = data[14];
        col4.w = data[15];
    }

    Vector4 Matrix44::GetColumn(size_t i) const
    {
        size_t val = i % 4;
        return Vector4(data[val], data[val + 4], data[val + 8], data[val + 12]);
    }

    Matrix44 Matrix44::Adjoint() const
    {
        Matrix44 result;
        result.data[0] = Determinant33(
                                       data[5], data[6], data[7],
                                       data[9], data[10], data[11],
                                       data[13], data[14], data[15]);
        result.data[1] = -Determinant33(
                                        data[1], data[2], data[3],
                                        data[9], data[10], data[11],
                                        data[13], data[14], data[15]);
        result.data[2] = Determinant33(
                                       data[1], data[2], data[3],
                                       data[5], data[6], data[7],
                                       data[13], data[14], data[15]);
        result.data[3] = -Determinant33(
                                        data[1], data[2], data[3],
                                        data[5], data[6], data[7],
                                        data[9], data[10], data[11]);
        result.data[4] = -Determinant33(
                                        data[4], data[6], data[7],
                                        data[8], data[10], data[11],
                                        data[12], data[14], data[15]);
        result.data[5] = Determinant33(
                                       data[0], data[2], data[3],
                                       data[8], data[10], data[11],
                                       data[12], data[14], data[15]);
        result.data[6] = -Determinant33(
                                        data[0], data[2], data[3],
                                        data[4], data[6], data[7],
                                        data[12], data[14], data[15]);
        result.data[7] = Determinant33(
                                       data[0], data[2], data[3],
                                       data[4], data[6], data[7],
                                       data[8], data[10], data[11]);
        result.data[8] = Determinant33(
                                       data[4], data[5], data[7],
                                       data[8], data[9], data[11],
                                       data[12], data[13], data[15]);
        result.data[9] = -Determinant33(
                                        data[0], data[1], data[3],
                                        data[8], data[9], data[11],
                                        data[12], data[13], data[15]);
        result.data[10] = Determinant33(
                                        data[0], data[1], data[3],
                                        data[4], data[5], data[7],
                                        data[12], data[13], data[15]);
        result.data[11] = -Determinant33(
                                         data[0], data[1], data[3],
                                         data[4], data[5], data[7],
                                         data[8], data[9], data[11]);
        result.data[12] = -Determinant33(
                                         data[4], data[5], data[6],
                                         data[8], data[9], data[10],
                                         data[12], data[13], data[14]);
        result.data[13] = Determinant33(
                                        data[0], data[1], data[2],
                                        data[8], data[9], data[10],
                                        data[12], data[13], data[14]);
        result.data[14] = -Determinant33(
                                         data[0], data[1], data[2],
                                         data[4], data[5], data[6],
                                         data[12], data[13], data[14]);
        result.data[15] = Determinant33(
                                        data[0], data[1], data[2],
                                        data[4], data[5], data[6],
                                        data[8], data[9], data[10]);
        return result;
    }

    Real Matrix44::Determinant() const
    {
        return
                data[0] * Determinant33(
                                        data[5], data[6], data[7],
                                        data[9], data[10], data[11],
                                        data[13], data[14], data[15]) -
                data[1] * Determinant33(
                                        data[4], data[6], data[7],
                                        data[8], data[10], data[11],
                                        data[12], data[14], data[15]) +
                data[2] * Determinant33(
                                        data[4], data[5], data[7],
                                        data[8], data[9], data[11],
                                        data[12], data[13], data[15]) -
                data[3] * Determinant33(
                                        data[4], data[5], data[6],
                                        data[8], data[9], data[10],
                                        data[12], data[13], data[14]);
    }

    Real Matrix44::Trace() const
    {
        return data[0] + data[5] + data[10] + data[15];
    }

    Matrix44 Matrix44::Inverse() const
    {
        Matrix44 result;
        Real     det = Determinant();
        if (Math::IsZero(det))
        {
            E5_ASSERT(false, "Inverse singular matrix");
            return result;
        }
        Real inv_det = 1.0f / det;
        result       = inv_det * Adjoint();
        return result;
    }

    Matrix44 Matrix44::Transpose() const
    {
        Matrix44 result;
        result.data[0]  = this->data[0];
        result.data[1]  = this->data[4];
        result.data[2]  = this->data[8];
        result.data[3]  = this->data[12];
        result.data[4]  = this->data[1];
        result.data[5]  = this->data[5];
        result.data[6]  = this->data[9];
        result.data[7]  = this->data[13];
        result.data[8]  = this->data[2];
        result.data[9]  = this->data[6];
        result.data[10] = this->data[10];
        result.data[11] = this->data[14];
        result.data[12] = this->data[3];
        result.data[13] = this->data[7];
        result.data[14] = this->data[11];
        result.data[15] = this->data[15];
        return result;
    }

    Matrix44 Matrix44::HadamardProduct(const Matrix44& rhs) const
    {
        Matrix44 result;
        for (size_t i = 0; i < 16; ++i)
        {
            result.data[i] = this->data[i] * rhs.data[i];
        }
        return result;
    }

    Vector3 Matrix44::TransformPoint(const Vector3& point)
    {
        Vector3 result;
        result.x = data[0] * point.x + data[4] * point.y + data[8] * point.z + data[12];
        result.y = data[1] * point.x + data[5] * point.y + data[9] * point.z + data[13];
        result.z = data[2] * point.x + data[6] * point.y + data[10] * point.z + data[14];
        return result;
    }

    Vector3 Matrix44::TransformVector(const Vector3& vector)
    {
        Vector3 result;
        result.x = data[0] * vector.x + data[4] * vector.y + data[8] * vector.z;
        result.y = data[1] * vector.x + data[5] * vector.y + data[9] * vector.z;
        result.z = data[2] * vector.x + data[6] * vector.y + data[10] * vector.z;
        return result;
    }

    Vector3 Matrix44::TransformVectorRotatingOrigin(const Vector3& vector, const Vector3& origin)
    {
        Vector3 vec = vector - origin;
        Vector3 result;
        result.x = data[0] * vec.x + data[4] * vec.y + data[8] * vec.z;
        result.y = data[1] * vec.x + data[5] * vec.y + data[9] * vec.z;
        result.z = data[2] * vec.x + data[6] * vec.y + data[10] * vec.z;
        return result + origin;
    }

    Vector3 Matrix44::GetPosition() const
    {
        return Vector3(data[3], data[7], data[11]);
    }

    Matrix44 Inverse(const Matrix44& mat)
    {
        return mat.Inverse();
    }

    Matrix44 Transpose(const Matrix44& mat)
    {
        return mat.Transpose();
    }

    Matrix44 HadamardProduct(const Matrix44& mat1, const Matrix44& mat2)
    {
        return mat1.HadamardProduct(mat2);
    }

    Matrix44& Matrix44::operator=(const Matrix44& rhs)
    {
        if (this != &rhs)
        {
            for (int i = 0; i < 16; ++i)
            {
                this->data[i] = rhs.data[i];
            }
        }
        return *this;
    }

    Real Matrix44::operator()(size_t i, size_t j) const
    {
        return data[4 * (i % 4) + (j % 4)];
    }

    Real& Matrix44::operator()(size_t i, size_t j)
    {
        return data[4 * (i % 4) + (j % 4)];
    }

    Real Matrix44::operator[](size_t i) const
    {
        return data[i];
    }

    Real& Matrix44::operator[](size_t i)
    {
        return data[i];
    }

    bool Matrix44::operator==(const Matrix44& rhs) const
    {
        for (size_t i = 0; i < 16; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return false;
        }
        return true;
    }

    bool Matrix44::operator!=(const Matrix44& rhs) const
    {
        for (size_t i = 0; i < 16; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return true;
        }
        return false;
    }

    Matrix44 Matrix44::operator+(const Matrix44& rhs) const
    {
        Matrix44 result;
        for (size_t i = 0; i < 16; ++i)
        {
            result.data[i] = data[i] + rhs.data[i];
        }
        return result;
    }

    Matrix44& Matrix44::operator+=(const Matrix44& rhs)
    {
        for (size_t i = 0; i < 16; ++i)
        {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    Matrix44 Matrix44::operator-(const Matrix44& rhs) const
    {
        Matrix44 result;
        for (size_t i = 0; i < 16; ++i)
        {
            result.data[i] = data[i] - rhs.data[i];
        }
        return result;
    }

    Matrix44& Matrix44::operator-=(const Matrix44& rhs)
    {
        for (size_t i = 0; i < 16; ++i)
        {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    Matrix44 Matrix44::operator-() const
    {
        Matrix44 result;
        for (size_t i = 0; i < 16; ++i)
        {
            result.data[i] = -data[i];
        }
        return result;
    }

    Matrix44 Matrix44::operator*(const Matrix44& matrix) const
    {
        Matrix44 result;
        result.data[0]  = data[0] * matrix.data[0] + data[1] * matrix.data[4] + data[2] * matrix.data[8] + data[3] * matrix.data[12];
        result.data[1]  = data[0] * matrix.data[1] + data[1] * matrix.data[5] + data[2] * matrix.data[9] + data[3] * matrix.data[13];
        result.data[2]  = data[0] * matrix.data[2] + data[1] * matrix.data[6] + data[2] * matrix.data[10] + data[3] * matrix.data[14];
        result.data[3]  = data[0] * matrix.data[3] + data[1] * matrix.data[7] + data[2] * matrix.data[11] + data[3] * matrix.data[15];
        result.data[4]  = data[4] * matrix.data[0] + data[5] * matrix.data[4] + data[6] * matrix.data[8] + data[7] * matrix.data[12];
        result.data[5]  = data[4] * matrix.data[1] + data[5] * matrix.data[5] + data[6] * matrix.data[9] + data[7] * matrix.data[13];
        result.data[6]  = data[4] * matrix.data[2] + data[5] * matrix.data[6] + data[6] * matrix.data[10] + data[7] * matrix.data[14];
        result.data[7]  = data[4] * matrix.data[3] + data[5] * matrix.data[7] + data[6] * matrix.data[11] + data[7] * matrix.data[15];
        result.data[8]  = data[8] * matrix.data[0] + data[9] * matrix.data[4] + data[10] * matrix.data[8] + data[11] * matrix.data[12];
        result.data[9]  = data[8] * matrix.data[1] + data[9] * matrix.data[5] + data[10] * matrix.data[9] + data[11] * matrix.data[13];
        result.data[10] = data[8] * matrix.data[2] + data[9] * matrix.data[6] + data[10] * matrix.data[10] + data[11] * matrix.data[14];
        result.data[11] = data[8] * matrix.data[3] + data[9] * matrix.data[7] + data[10] * matrix.data[11] + data[11] * matrix.data[15];
        result.data[12] = data[12] * matrix.data[0] + data[13] * matrix.data[4] + data[14] * matrix.data[8] + data[15] * matrix.data[12];
        result.data[13] = data[12] * matrix.data[1] + data[13] * matrix.data[5] + data[14] * matrix.data[9] + data[15] * matrix.data[13];
        result.data[14] = data[12] * matrix.data[2] + data[13] * matrix.data[6] + data[14] * matrix.data[10] + data[15] * matrix.data[14];
        result.data[15] = data[12] * matrix.data[3] + data[13] * matrix.data[7] + data[14] * matrix.data[11] + data[15] * matrix.data[15];
        return result;
    }

    Matrix44& Matrix44::operator*=(const Matrix44& matrix)
    {
        Matrix44 result;
        result.data[0]  = data[0] * matrix.data[0] + data[1] * matrix.data[4] + data[2] * matrix.data[8] + data[3] * matrix.data[12];
        result.data[1]  = data[0] * matrix.data[1] + data[1] * matrix.data[5] + data[2] * matrix.data[9] + data[3] * matrix.data[13];
        result.data[2]  = data[0] * matrix.data[2] + data[1] * matrix.data[6] + data[2] * matrix.data[10] + data[3] * matrix.data[14];
        result.data[3]  = data[0] * matrix.data[3] + data[1] * matrix.data[7] + data[2] * matrix.data[11] + data[3] * matrix.data[15];
        result.data[4]  = data[4] * matrix.data[0] + data[5] * matrix.data[4] + data[6] * matrix.data[8] + data[7] * matrix.data[12];
        result.data[5]  = data[4] * matrix.data[1] + data[5] * matrix.data[5] + data[6] * matrix.data[9] + data[7] * matrix.data[13];
        result.data[6]  = data[4] * matrix.data[2] + data[5] * matrix.data[6] + data[6] * matrix.data[10] + data[7] * matrix.data[14];
        result.data[7]  = data[4] * matrix.data[3] + data[5] * matrix.data[7] + data[6] * matrix.data[11] + data[7] * matrix.data[15];
        result.data[8]  = data[8] * matrix.data[0] + data[9] * matrix.data[4] + data[10] * matrix.data[8] + data[11] * matrix.data[12];
        result.data[9]  = data[8] * matrix.data[1] + data[9] * matrix.data[5] + data[10] * matrix.data[9] + data[11] * matrix.data[13];
        result.data[10] = data[8] * matrix.data[2] + data[9] * matrix.data[6] + data[10] * matrix.data[10] + data[11] * matrix.data[14];
        result.data[11] = data[8] * matrix.data[3] + data[9] * matrix.data[7] + data[10] * matrix.data[11] + data[11] * matrix.data[15];
        result.data[12] = data[12] * matrix.data[0] + data[13] * matrix.data[4] + data[14] * matrix.data[8] + data[15] * matrix.data[12];
        result.data[13] = data[12] * matrix.data[1] + data[13] * matrix.data[5] + data[14] * matrix.data[9] + data[15] * matrix.data[13];
        result.data[14] = data[12] * matrix.data[2] + data[13] * matrix.data[6] + data[14] * matrix.data[10] + data[15] * matrix.data[14];
        result.data[15] = data[12] * matrix.data[3] + data[13] * matrix.data[7] + data[14] * matrix.data[11] + data[15] * matrix.data[15];
        for (size_t i = 0; i < 16; ++i)
        {
            data[i] = result.data[i];
        }
        return *this;
    }

    Vector4 Matrix44::operator*(const Vector4& vector) const
    {
        Vector4 result;
        result.x = data[0] * vector.x + data[1] * vector.y + data[2] * vector.z + data[3] * vector.w;
        result.y = data[4] * vector.x + data[5] * vector.y + data[6] * vector.z + data[7] * vector.w;
        result.z = data[8] * vector.x + data[9] * vector.y + data[10] * vector.z + data[11] * vector.w;
        result.w = data[12] * vector.x + data[13] * vector.y + data[14] * vector.z + data[15] * vector.w;
        return result;
    }

    Vector4 operator*(const Vector4& vector, const Matrix44& matrix)
    {
        Vector4 result;
        result.x = matrix.data[0] * vector.x + matrix.data[4] * vector.y + matrix.data[8] * vector.z + matrix.data[12] * vector.w;
        result.y = matrix.data[1] * vector.x + matrix.data[5] * vector.y + matrix.data[9] * vector.z + matrix.data[13] * vector.w;
        result.z = matrix.data[2] * vector.x + matrix.data[6] * vector.y + matrix.data[10] * vector.z + matrix.data[14] * vector.w;
        result.w = matrix.data[3] * vector.x + matrix.data[7] * vector.y + matrix.data[11] * vector.z + matrix.data[15] * vector.w;
        return result;
    }

    Matrix44 Matrix44::operator*(Real real) const
    {
        Matrix44 result;
        for (size_t i = 0; i < 16; ++i)
        {
            result.data[i] = real * data[i];
        }
        return result;
    }

    Matrix44& Matrix44::operator*=(Real real)
    {
        for (size_t i = 0; i < 16; ++i)
        {
            data[i] *= real;
        }
        return *this;
    }

    Matrix44 operator*(Real real, const Matrix44& matrix)
    {
        Matrix44 result;
        for (size_t i = 0; i < 16; ++i)
        {
            result.data[i] = real * matrix.data[i];
        }
        return result;
    }

    std::ostream& operator<<(std::ostream& os, const Matrix44& rhs)
    {
        os << "|" << rhs.data[0] << ", " << rhs.data[1] << ", " << rhs.data[2] << ", " << rhs.data[3] << "|\n";
        os << "|" << rhs.data[4] << ", " << rhs.data[5] << ", " << rhs.data[6] << ", " << rhs.data[7] << "|\n";
        os << "|" << rhs.data[8] << ", " << rhs.data[9] << ", " << rhs.data[10] << ", " << rhs.data[11] << "|\n";
        os << "|" << rhs.data[12] << ", " << rhs.data[13] << ", " << rhs.data[14] << ", " << rhs.data[15] << "|";
        return os;
    }

    Matrix44 Matrix44::Identity()
    {
        return Matrix44();
    }

    Matrix44 Matrix44::Zero()
    {
        return Matrix44(
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f);
    }

    Real Matrix44::Determinant33(Real c0, Real c1, Real c2, Real c3, Real c4, Real c5, Real c6, Real c7, Real c8)
    {
        return c0 * (c4 * c8 - c5 * c7) + c1 * (c5 * c6 - c3 * c8) + c2 * (c3 * c7 - c4 * c6);
    }
}
