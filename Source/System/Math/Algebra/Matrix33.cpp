#include "Matrix33.hpp"
#include "Quaternion.hpp"
#include "../Utility/Utility.hpp"
#include "Vector2.hpp"
#include <ostream>

#include "Matrix44.hpp"
#include "../../Core/Utility/CoreUtility.hpp"
#include "../Utility/VectorDef.hpp"

namespace CS460
{
    Matrix33::Matrix33(Real c0, Real c1, Real c2, Real c3, Real c4, Real c5, Real c6, Real c7, Real c8)
    {
        data[0] = c0;
        data[1] = c1;
        data[2] = c2;
        data[3] = c3;
        data[4] = c4;
        data[5] = c5;
        data[6] = c6;
        data[7] = c7;
        data[8] = c8;
    }

    Matrix33::Matrix33(const Matrix33& rhs)
    {
        data[0] = rhs.data[0];
        data[1] = rhs.data[1];
        data[2] = rhs.data[2];
        data[3] = rhs.data[3];
        data[4] = rhs.data[4];
        data[5] = rhs.data[5];
        data[6] = rhs.data[6];
        data[7] = rhs.data[7];
        data[8] = rhs.data[8];
    }

    Matrix33::Matrix33(const Quaternion& quaternion)
    {
        SetRotation(quaternion);
    }

    Matrix33::Matrix33(const EulerAngle& euler_angle)
    {
        SetRotation(euler_angle);
    }

    Matrix33::Matrix33(const AxisRadian& axis_radian)
    {
        SetRotation(axis_radian);
    }

    Matrix33::Matrix33(const Vector3& axis, Real radian)
    {
        SetRotation(axis, radian);
    }

    Matrix33::Matrix33(const Matrix44& matrix)
    {
        data[0] = matrix.data[0];
        data[1] = matrix.data[1];
        data[2] = matrix.data[2];

        data[3] = matrix.data[4];
        data[4] = matrix.data[5];
        data[5] = matrix.data[6];

        data[6] = matrix.data[8];
        data[7] = matrix.data[9];
        data[8] = matrix.data[10];
    }

    Matrix33::~Matrix33()
    {
    }

    void Matrix33::SetRows(const Vector3& row1, const Vector3& row2, const Vector3& row3)
    {
        data[0] = row1.x;
        data[1] = row1.y;
        data[2] = row1.z;
        data[3] = row2.x;
        data[4] = row2.y;
        data[5] = row2.z;
        data[6] = row3.x;
        data[7] = row3.y;
        data[8] = row3.z;
    }

    void Matrix33::SetColumns(const Vector3& col1, const Vector3& col2, const Vector3& col3)
    {
        data[0] = col1.x;
        data[1] = col2.x;
        data[2] = col3.x;
        data[3] = col1.y;
        data[4] = col2.y;
        data[5] = col3.y;
        data[6] = col1.z;
        data[7] = col2.z;
        data[8] = col3.z;
    }

    void Matrix33::SetDiagonal(Real a, Real b, Real c)
    {
        data[0] = a;
        data[4] = b;
        data[8] = c;
    }

    void Matrix33::SetClean()
    {
        for (size_t i = 0; i < 9; ++i)
        {
            if (Math::IsZero(data[i]))
                data[i] = 0.0f;
        }
    }

    void Matrix33::SetIdentity()
    {
        data[0] = 1.0f;
        data[1] = 0.0f;
        data[2] = 0.0f;
        data[3] = 0.0f;
        data[4] = 1.0f;
        data[5] = 0.0f;
        data[6] = 0.0f;
        data[7] = 0.0f;
        data[8] = 1.0f;
    }

    void Matrix33::SetInverse()
    {
        *this = this->Inverse();
    }

    void Matrix33::SetTranspose()
    {
        *this = this->Transpose();
    }

    void Matrix33::SetZero()
    {
        data[0] = 0.0f;
        data[1] = 0.0f;
        data[2] = 0.0f;
        data[3] = 0.0f;
        data[4] = 0.0f;
        data[5] = 0.0f;
        data[6] = 0.0f;
        data[7] = 0.0f;
        data[8] = 0.0f;
    }

    void Matrix33::SetRotation(const Quaternion& quaternion)
    {
        if (quaternion.IsUnit())
        {
            Real xs = quaternion.i + quaternion.i;
            Real ys = quaternion.j + quaternion.j;
            Real zs = quaternion.k + quaternion.k;
            Real wx = quaternion.r * xs;
            Real wy = quaternion.r * ys;
            Real wz = quaternion.r * zs;
            Real xx = quaternion.i * xs;
            Real xy = quaternion.i * ys;
            Real xz = quaternion.i * zs;
            Real yy = quaternion.j * ys;
            Real yz = quaternion.j * zs;
            Real zz = quaternion.k * zs;
            data[0] = 1.0f - (yy + zz);
            data[1] = xy - wz;
            data[2] = xz + wy;
            data[3] = xy + wz;
            data[4] = 1.0f - (xx + zz);
            data[5] = yz - wx;
            data[6] = xz - wy;
            data[7] = yz + wx;
            data[8] = 1.0f - (xx + yy);
        }
    }

    void Matrix33::SetRotation(const EulerAngle& euler_angle)
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
        data[3]    = (sin_x * sin_y * cos_z) + (cos_x * sin_z);
        data[4]    = -(sin_x * sin_y * sin_z) + (cos_x * cos_z);
        data[5]    = -(sin_x * cos_y);
        data[6]    = -(cos_x * sin_y * cos_z) + (sin_x * sin_z);
        data[7]    = (cos_x * sin_y * sin_z) + (sin_x * cos_z);
        data[8]    = (cos_x * cos_y);
    }

    void Matrix33::SetRotation(const AxisRadian& axis_radian)
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
        data[3]           = (txy + sz);
        data[4]           = (ty * unit_axis.y + cos);
        data[5]           = (tyz - sx);
        data[6]           = (txz - sy);
        data[7]           = (tyz + sx);
        data[8]           = (tz * unit_axis.z + cos);
    }

    void Matrix33::SetRotation(const Vector3& axis, Real radian)
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
        data[3]           = (txy + sz);
        data[4]           = (ty * unit_axis.y + cos);
        data[5]           = (tyz - sx);
        data[6]           = (txz - sy);
        data[7]           = (tyz + sx);
        data[8]           = (tz * unit_axis.z + cos);
    }

    void Matrix33::SetRotationX(Real radian)
    {
        Real sin = sinf(radian);
        Real cos = cosf(radian);
        data[0]  = 1.0f;
        data[1]  = 0.0f;
        data[2]  = 0.0f;
        data[3]  = 0.0f;
        data[4]  = cos;
        data[5]  = -sin;
        data[6]  = 0.0f;
        data[7]  = sin;
        data[8]  = cos;
    }

    void Matrix33::SetRotationY(Real radian)
    {
        Real sin = sinf(radian);
        Real cos = cosf(radian);
        data[0]  = cos;
        data[1]  = 0.0f;
        data[2]  = sin;
        data[3]  = 0.0f;
        data[4]  = 1.0f;
        data[5]  = 0.0f;
        data[6]  = -sin;
        data[7]  = 0.0f;
        data[8]  = cos;
    }

    void Matrix33::SetRotationZ(Real radian)
    {
        Real sin = sinf(radian);
        Real cos = cosf(radian);
        data[0]  = cos;
        data[1]  = -sin;
        data[2]  = 0.0f;
        data[3]  = sin;
        data[4]  = cos;
        data[5]  = 0.0f;
        data[6]  = 0.0f;
        data[7]  = 0.0f;
        data[8]  = 1.0f;
    }

    void Matrix33::SetScale(const Vector3& scale)
    {
        data[0] = scale.x;
        data[1] = 0.0f;
        data[2] = 0.0f;
        data[3] = 0.0f;
        data[4] = scale.y;
        data[5] = 0.0f;
        data[6] = 0.0f;
        data[7] = 0.0f;
        data[8] = scale.z;
    }

    void Matrix33::SetTranslation(const Vector2& translation)
    {
        data[0] = 1.0f;
        data[1] = 0.0f;
        data[2] = translation.x;
        data[3] = 0.0f;
        data[4] = 1.0f;
        data[5] = translation.y;
        data[6] = 0.0f;
        data[7] = 0.0f;
        data[8] = 1.0f;
    }

    void Matrix33::SetSkew(const Vector3& v)
    {
        data[0] = 0.0f;
        data[1] = -v.z;
        data[2] = v.y;
        data[3] = v.z;
        data[4] = 0.0f;
        data[5] = -v.x;
        data[6] = -v.y;
        data[7] = v.x;
        data[8] = 0.0f;
    }

    bool Matrix33::IsZero() const
    {
        for (size_t i = 0; i < 9; ++i)
        {
            if (Math::IsZero(data[i]) == false)
                return false;
        }
        return true;
    }

    bool Matrix33::IsIdentity() const
    {
        return Math::IsEqual(1.0f, data[0])
                && Math::IsEqual(1.0f, data[4])
                && Math::IsEqual(1.0f, data[8])
                && Math::IsZero(data[1])
                && Math::IsZero(data[2])
                && Math::IsZero(data[3])
                && Math::IsZero(data[5])
                && Math::IsZero(data[6])
                && Math::IsZero(data[7]);
    }

    bool Matrix33::IsEqual(const Matrix33& rhs) const
    {
        for (size_t i = 0; i < 9; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return false;
        }
        return true;
    }

    bool Matrix33::IsNotEqual(const Matrix33& rhs) const
    {
        for (size_t i = 0; i < 9; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return true;
        }
        return false;
    }

    void Matrix33::GetRows(Vector3& row1, Vector3& row2, Vector3& row3) const
    {
        row1.x = data[0];
        row1.y = data[1];
        row1.z = data[2];
        row2.x = data[3];
        row2.y = data[4];
        row2.z = data[5];
        row3.x = data[6];
        row3.y = data[7];
        row3.z = data[8];
    }

    Vector3 Matrix33::GetRow(size_t i) const
    {
        size_t val = i % 3;
        return Vector3(data[3 * val], data[3 * val + 1], data[3 * val + 2]);
    }

    void Matrix33::GetColumns(Vector3& col1, Vector3& col2, Vector3& col3) const
    {
        col1.x = data[0];
        col2.x = data[1];
        col3.x = data[2];
        col1.y = data[3];
        col2.y = data[4];
        col3.y = data[5];
        col1.z = data[6];
        col2.z = data[7];
        col3.z = data[8];
    }

    Vector3 Matrix33::GetColumn(size_t i) const
    {
        size_t val = i % 3;
        return Vector3(data[val], data[val + 3], data[val + 6]);
    }

    Quaternion Matrix33::ToQuaternion() const
    {
        Quaternion result;
        Real       trace = Trace();
        if (trace > 0.0f)
        {
            Real scalar          = sqrtf(trace + 1.0f);
            result.r        = scalar * 0.5f;
            Real multiplier = 0.25f / scalar;
            result.i        = ((*this)(2, 1) - (*this)(1, 2)) * multiplier;
            result.j        = ((*this)(0, 2) - (*this)(2, 0)) * multiplier;
            result.k        = ((*this)(1, 0) - (*this)(0, 1)) * multiplier;
        }
        else
        {
            size_t _i = 0;
            if ((*this)(1, 1) > (*this)(0, 0))
            {
                _i = 1;
            }
            if ((*this)(2, 2) > (*this)(_i, _i))
            {
                _i = 2;
            }
            size_t _j       = (_i + 1) % 3;
            size_t _k       = (_j + 1) % 3;
            Real   s        = sqrtf((*this)(_i, _i) - (*this)(_j, _j) - (*this)(_k, _k) + 1.0f);
            result[_i]      = 0.5f * s;
            Real multiplier = 0.5f / s;
            result.r        = ((*this)(_k, _j) - (*this)(_j, _k)) * multiplier;
            result[_j]      = ((*this)(_j, _i) + (*this)(_i, _j)) * multiplier;
            result[_k]      = ((*this)(_k, _i) + (*this)(_i, _k)) * multiplier;
        }
        return result;
    }

    AxisRadian Matrix33::ToAxisRadian() const
    {
        Real    trace     = data[0] + data[4] + data[8];
        Real    cos_theta = 0.5f * (trace - 1.0f);
        Real    radian    = acosf(cos_theta);
        Vector3 axis;
        // angle is zero, axis can be anything
        if (Math::IsZero(radian))
        {
            axis = Math::Vector3::Y_AXIS;
        }
            // standard case
        else if (radian < Math::PI - Math::EPSILON)
        {
            axis.Set(data[7] - data[5], data[2] - data[6], data[3] - data[1]);
            axis.SetNormalize();
        }
            // angle is 180 degrees
        else
        {
            size_t i = 0;
            if (data[4] > data[0])
            {
                i = 1;
            }
            if (data[8] > data[i + 3 * i])
            {
                i = 2;
            }
            size_t j        = (i + 1) % 3;
            size_t k        = (j + 1) % 3;
            Real   s        = sqrtf(data[i + 3 * i] - data[j + 3 * j] - data[k + 3 * k] + 1.0f);
            axis[i]         = 0.5f * s;
            Real multiplier = 1.0f / s;
            axis[j]         = (data[3 * i + j]) * multiplier;
            axis[k]         = (data[3 * k + i]) * multiplier;
        }
        return AxisRadian(axis, radian);
    }

    EulerAngle Matrix33::ToEulerAngle() const
    {
        Real cos_x, sin_x, cos_z, sin_z;
        Real sin_y = data[2];
        Real cos_y = sqrtf(1.0f - sin_y * sin_y);
        if (Math::IsZero(cos_y) == false)
        {
            Real factor = 1.0f / cos_y;
            sin_x       = -data[5] * factor;
            cos_x       = data[8] * factor;
            sin_z       = -data[1] * factor;
            cos_z       = data[0] * factor;
        }
        else // x and z axes aligned
        {
            sin_z = 0.0f;
            cos_z = 1.0f;
            sin_x = data[7];
            cos_x = data[4];
        }
        Real x_rotation = atan2f(sin_x, cos_x);
        Real y_rotation = atan2f(sin_y, cos_y);
        Real z_rotation = atan2f(sin_z, cos_z);
        return EulerAngle(x_rotation, y_rotation, z_rotation);
    }

    Matrix33 Matrix33::Adjoint() const
    {
        Matrix33 result;
        // compute transpose of cofactors
        result.data[0] = data[4] * data[8] - data[7] * data[5];
        result.data[3] = data[6] * data[5] - data[3] * data[8];
        result.data[6] = data[3] * data[7] - data[6] * data[4];
        result.data[1] = data[7] * data[2] - data[1] * data[8];
        result.data[4] = data[0] * data[8] - data[6] * data[2];
        result.data[7] = data[6] * data[1] - data[0] * data[7];
        result.data[2] = data[1] * data[5] - data[4] * data[2];
        result.data[5] = data[3] * data[2] - data[0] * data[5];
        result.data[8] = data[0] * data[4] - data[3] * data[1];
        return result;
    }

    Real Matrix33::Determinant() const
    {
        return data[0] * (data[4] * data[8] - data[5] * data[7])
                + data[1] * (data[5] * data[6] - data[3] * data[8])
                + data[2] * (data[3] * data[7] - data[4] * data[6]);
    }

    Real Matrix33::Trace() const
    {
        return data[0] + data[4] + data[8];
    }

    Matrix33 Matrix33::Inverse() const
    {
        Matrix33 result;
        // compute determinant
        Real cofactor0 = this->data[4] * this->data[8] - this->data[5] * this->data[7];
        Real cofactor3 = this->data[5] * this->data[6] - this->data[3] * this->data[8];
        Real cofactor6 = this->data[3] * this->data[7] - this->data[4] * this->data[6];
        Real det       = this->data[0] * cofactor0 + this->data[1] * cofactor3 + this->data[2] * cofactor6;
        if (Math::IsZero(det))
        {
            //E5_ASSERT(false, "Inverse singular matrix");
            return result;
        }
        // create adjoint matrix and multiply by 1/det to get inverse
        Real inv_det   = 1.0f / det;
        result.data[0] = inv_det * cofactor0;
        result.data[3] = inv_det * cofactor3;
        result.data[6] = inv_det * cofactor6;
        result.data[1] = inv_det * (this->data[2] * this->data[7] - this->data[1] * this->data[8]);
        result.data[4] = inv_det * (this->data[0] * this->data[8] - this->data[2] * this->data[6]);
        result.data[7] = inv_det * (this->data[1] * this->data[6] - this->data[0] * this->data[7]);
        result.data[2] = inv_det * (this->data[1] * this->data[5] - this->data[2] * this->data[4]);
        result.data[5] = inv_det * (this->data[2] * this->data[3] - this->data[0] * this->data[5]);
        result.data[8] = inv_det * (this->data[0] * this->data[4] - this->data[1] * this->data[3]);
        return result;
    }

    Matrix33 Matrix33::Transpose() const
    {
        Matrix33 result;
        result.data[0] = this->data[0];
        result.data[1] = this->data[3];
        result.data[2] = this->data[6];
        result.data[3] = this->data[1];
        result.data[4] = this->data[4];
        result.data[5] = this->data[7];
        result.data[6] = this->data[2];
        result.data[7] = this->data[5];
        result.data[8] = this->data[8];
        return result;
    }

    Matrix33 Matrix33::HadamardProduct(const Matrix33& rhs) const
    {
        return Matrix33(
                        data[0] * rhs.data[0], data[1] * rhs.data[1], data[2] * rhs.data[2],
                        data[3] * rhs.data[3], data[4] * rhs.data[4], data[5] * rhs.data[5],
                        data[6] * rhs.data[6], data[7] * rhs.data[7], data[8] * rhs.data[8]);
    }

    Matrix33 Inverse(const Matrix33& mat)
    {
        return mat.Inverse();
    }

    Matrix33 Transpose(const Matrix33& mat)
    {
        return mat.Transpose();
    }

    Matrix33 HadamardProduct(const Matrix33& mat1, const Matrix33& mat2)
    {
        return mat1.HadamardProduct(mat2);
    }

    Matrix33 Skew(const Vector3& v)
    {
        return Matrix33(
                        0.0f, -v.z, v.y,
                        v.z, 0.0f, -v.x,
                        -v.y, v.x, 0.0f
                       );
    }

    Matrix33& Matrix33::operator=(const Matrix33& rhs)
    {
        if (this != &rhs)
        {
            for (int i = 0; i < 9; ++i)
            {
                this->data[i] = rhs.data[i];
            }
        }
        return *this;
    }

    Matrix33& Matrix33::operator=(const Matrix44& rhs)
    {
        data[0] = rhs.data[0];
        data[1] = rhs.data[1];
        data[2] = rhs.data[2];

        data[3] = rhs.data[4];
        data[4] = rhs.data[5];
        data[5] = rhs.data[6];

        data[6] = rhs.data[8];
        data[7] = rhs.data[9];
        data[8] = rhs.data[10];
        return *this;
    }

    Real Matrix33::operator()(size_t i, size_t j) const
    {
        return data[3 * (i % 3) + (j % 3)];
    }

    Real& Matrix33::operator()(size_t i, size_t j)
    {
        return data[3 * (i % 3) + (j % 3)];
    }

    Real Matrix33::operator[](size_t i) const
    {
        return data[i];
    }

    Real& Matrix33::operator[](size_t i)
    {
        return data[i];
    }

    bool Matrix33::operator==(const Matrix33& rhs) const
    {
        for (size_t i = 0; i < 9; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return false;
        }
        return true;
    }

    bool Matrix33::operator!=(const Matrix33& rhs) const
    {
        for (size_t i = 0; i < 9; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return true;
        }
        return false;
    }

    Matrix33 Matrix33::operator+(const Matrix33& rhs) const
    {
        Matrix33 result;
        for (size_t i = 0; i < 9; ++i)
        {
            result.data[i] = data[i] + rhs.data[i];
        }
        return result;
    }

    Matrix33& Matrix33::operator+=(const Matrix33& rhs)
    {
        for (size_t i = 0; i < 9; ++i)
        {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    Matrix33 Matrix33::operator-(const Matrix33& rhs) const
    {
        Matrix33 result;
        for (size_t i = 0; i < 9; ++i)
        {
            result.data[i] = data[i] - rhs.data[i];
        }
        return result;
    }

    Matrix33& Matrix33::operator-=(const Matrix33& rhs)
    {
        for (size_t i = 0; i < 9; ++i)
        {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    Matrix33 Matrix33::operator-() const
    {
        Matrix33 result;
        for (size_t i = 0; i < 9; ++i)
        {
            result.data[i] = -data[i];
        }
        return result;
    }

    Matrix33 Matrix33::operator*(const Matrix33& matrix) const
    {
        Matrix33 result;
        result.data[0] = data[0] * matrix.data[0] + data[1] * matrix.data[3] + data[2] * matrix.data[6];
        result.data[1] = data[0] * matrix.data[1] + data[1] * matrix.data[4] + data[2] * matrix.data[7];
        result.data[2] = data[0] * matrix.data[2] + data[1] * matrix.data[5] + data[2] * matrix.data[8];
        result.data[3] = data[3] * matrix.data[0] + data[4] * matrix.data[3] + data[5] * matrix.data[6];
        result.data[4] = data[3] * matrix.data[1] + data[4] * matrix.data[4] + data[5] * matrix.data[7];
        result.data[5] = data[3] * matrix.data[2] + data[4] * matrix.data[5] + data[5] * matrix.data[8];
        result.data[6] = data[6] * matrix.data[0] + data[7] * matrix.data[3] + data[8] * matrix.data[6];
        result.data[7] = data[6] * matrix.data[1] + data[7] * matrix.data[4] + data[8] * matrix.data[7];
        result.data[8] = data[6] * matrix.data[2] + data[7] * matrix.data[5] + data[8] * matrix.data[8];
        return result;
    }

    Matrix33& Matrix33::operator*=(const Matrix33& matrix)
    {
        Matrix33 result;
        result.data[0] = data[0] * matrix.data[0] + data[1] * matrix.data[3] + data[2] * matrix.data[6];
        result.data[1] = data[0] * matrix.data[1] + data[1] * matrix.data[4] + data[2] * matrix.data[7];
        result.data[2] = data[0] * matrix.data[2] + data[1] * matrix.data[5] + data[2] * matrix.data[8];
        result.data[3] = data[3] * matrix.data[0] + data[4] * matrix.data[3] + data[5] * matrix.data[6];
        result.data[4] = data[3] * matrix.data[1] + data[4] * matrix.data[4] + data[5] * matrix.data[7];
        result.data[5] = data[3] * matrix.data[2] + data[4] * matrix.data[5] + data[5] * matrix.data[8];
        result.data[6] = data[6] * matrix.data[0] + data[7] * matrix.data[3] + data[8] * matrix.data[6];
        result.data[7] = data[6] * matrix.data[1] + data[7] * matrix.data[4] + data[8] * matrix.data[7];
        result.data[8] = data[6] * matrix.data[2] + data[7] * matrix.data[5] + data[8] * matrix.data[8];
        for (size_t i = 0; i < 9; ++i)
        {
            data[i] = result.data[i];
        }
        return *this;
    }

    Vector3 Matrix33::operator*(const Vector3& vector) const
    {
        Vector3 result;
        result.x = data[0] * vector.x + data[1] * vector.y + data[2] * vector.z;
        result.y = data[3] * vector.x + data[4] * vector.y + data[5] * vector.z;
        result.z = data[6] * vector.x + data[7] * vector.y + data[8] * vector.z;
        return result;
    }

    Vector3 operator*(const Vector3& vector, const Matrix33& matrix)
    {
        Vector3 result;
        result.x = matrix.data[0] * vector.x + matrix.data[3] * vector.y + matrix.data[6] * vector.z;
        result.y = matrix.data[1] * vector.x + matrix.data[4] * vector.y + matrix.data[7] * vector.z;
        result.z = matrix.data[2] * vector.x + matrix.data[5] * vector.y + matrix.data[8] * vector.z;
        return result;
    }

    Matrix33 Matrix33::operator*(Real real) const
    {
        Matrix33 result;
        result.data[0] = data[0] * real;
        result.data[1] = data[1] * real;
        result.data[2] = data[2] * real;
        result.data[3] = data[3] * real;
        result.data[4] = data[4] * real;
        result.data[5] = data[5] * real;
        result.data[6] = data[6] * real;
        result.data[7] = data[7] * real;
        result.data[8] = data[8] * real;
        return result;
    }

    Matrix33& Matrix33::operator*=(Real real)
    {
        data[0] *= real;
        data[1] *= real;
        data[2] *= real;
        data[3] *= real;
        data[4] *= real;
        data[5] *= real;
        data[6] *= real;
        data[7] *= real;
        data[8] *= real;
        return *this;
    }

    Matrix33 operator*(Real real, const Matrix33& matrix)
    {
        Matrix33 result;
        result.data[0] = matrix.data[0] * real;
        result.data[1] = matrix.data[1] * real;
        result.data[2] = matrix.data[2] * real;
        result.data[3] = matrix.data[3] * real;
        result.data[4] = matrix.data[4] * real;
        result.data[5] = matrix.data[5] * real;
        result.data[6] = matrix.data[6] * real;
        result.data[7] = matrix.data[7] * real;
        result.data[8] = matrix.data[8] * real;
        return result;
    }

    std::ostream& operator<<(std::ostream& os, const Matrix33& rhs)
    {
        os << "|" << rhs.data[0] << ", " << rhs.data[1] << ", " << rhs.data[2] << "|\n";
        os << "|" << rhs.data[3] << ", " << rhs.data[4] << ", " << rhs.data[5] << "|\n";
        os << "|" << rhs.data[6] << ", " << rhs.data[7] << ", " << rhs.data[8] << "|";
        return os;
    }

    Matrix33 Matrix33::Identity()
    {
        return Matrix33();
    }

    Matrix33 Matrix33::Zero()
    {
        return Matrix33(
                        0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f);
    }
}
