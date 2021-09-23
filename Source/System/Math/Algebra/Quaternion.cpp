#include "Quaternion.hpp"
#include "Matrix33.hpp"
#include "Vector3.hpp"
#include "..//Utility/Utility.hpp"
#include <ostream>
#include "../Utility/VectorDef.hpp"
#include <sstream>

namespace CS460
{
    Quaternion::Quaternion(Real r, Real i, Real j, Real k)
        : r(r), i(i), j(j), k(k)
    {
    }

    Quaternion::Quaternion(const Vector3& from, const Vector3& to)
    {
        Set(from, to);
    }

    Quaternion::Quaternion(const Vector3& vector)
    {
        Set(vector);
    }

    Quaternion::Quaternion(const Matrix33& rotation_matrix)
    {
        Set(rotation_matrix);
    }

    Quaternion::Quaternion(const AxisRadian& axis_radian)
    {
        Set(axis_radian);
    }

    Quaternion::Quaternion(const EulerAngle& euler_angle)
    {
        Set(euler_angle);
    }

    Quaternion::Quaternion(const Vector3& axis, Real radian)
    {
        Set(axis, radian);
    }

    Quaternion::Quaternion(const Quaternion& rhs)
        : r(rhs.r), i(rhs.i), j(rhs.j), k(rhs.k)
    {
    }

    Quaternion::~Quaternion()
    {
    }

    void Quaternion::Set(Real _r, Real _i, Real _j, Real _k)
    {
        r = _r;
        i = _i;
        j = _j;
        k = _k;
    }

    void Quaternion::Set(const Vector3& from, const Vector3& to)
    {
        Vector3 axis = from.CrossProduct(to);
        Set(from.DotProduct(to), axis.x, axis.y, axis.z);
        SetNormalize();
        r += 1.0f;
        if (r <= Math::EPSILON)
        {
            if (from.z * from.z > from.x * from.x)
                Set(0.0f, 0.0f, from.z, -from.y);
            else
                Set(0.0f, from.y, -from.x, 0.0f);
        }
        SetNormalize();
    }

    void Quaternion::Set(const Vector3& axis, Real radian)
    {
        // if axis of rotation is zero vector, just set to identity quat
        Real length = axis.LengthSquared();
        if (Math::IsZero(length) == true)
        {
            SetIdentity();
            return;
        }
        // take half-angle
        Real half_rad  = radian * 0.5f;
        Real sin_theta = sinf(half_rad);
        Real cos_theta = cosf(half_rad);
        Real scale     = sin_theta / sqrtf(length);
        r              = cos_theta;
        i              = scale * axis.x;
        j              = scale * axis.y;
        k              = scale * axis.z;
    }

    void Quaternion::Set(const Vector3& vector)
    {
        r = 0.0f;
        i = vector.x;
        j = vector.y;
        k = vector.z;
    }

    void Quaternion::Set(const Matrix33& rotation_matrix)
    {
        Real trace = rotation_matrix.Trace();
        if (trace > 0.0f)
        {
            Real scalar     = sqrtf(trace + 1.0f);
            r               = scalar * 0.5f;
            Real multiplier = 0.25f / scalar;
            i               = (rotation_matrix(2, 1) - rotation_matrix(1, 2)) * multiplier;
            j               = (rotation_matrix(0, 2) - rotation_matrix(2, 0)) * multiplier;
            k               = (rotation_matrix(1, 0) - rotation_matrix(0, 1)) * multiplier;
        }
        else
        {
            size_t _i = 0;
            if (rotation_matrix(1, 1) > rotation_matrix(0, 0))
            {
                _i = 1;
            }
            if (rotation_matrix(2, 2) > rotation_matrix(_i, _i))
            {
                _i = 2;
            }
            size_t _j       = (_i + 1) % 3;
            size_t _k       = (_j + 1) % 3;
            Real   scalar   = sqrtf(rotation_matrix(_i, _i) - rotation_matrix(_j, _j) - rotation_matrix(_k, _k) + 1.0f);
            (*this)[_i]     = 0.5f * scalar;
            Real multiplier = 0.25f / scalar;
            r               = (rotation_matrix(_k, _j) - rotation_matrix(_j, _k)) * multiplier;
            (*this)[_j]     = (rotation_matrix(_j, _i) + rotation_matrix(_i, _j)) * multiplier;
            (*this)[_k]     = (rotation_matrix(_k, _i) + rotation_matrix(_i, _k)) * multiplier;
        }
    }

    void Quaternion::Set(const AxisRadian& axis_radian)
    {
        // if axis of rotation is zero vector, just set to identity quat
        Real length = axis_radian.axis.LengthSquared();
        if (Math::IsZero(length) == true)
        {
            SetIdentity();
            return;
        }
        // take half-angle
        Real half_rad  = axis_radian.radian * 0.5f;
        Real sin_theta = sinf(half_rad);
        Real cos_theta = cosf(half_rad);
        Real scale     = sin_theta / sqrtf(length);
        r              = cos_theta;
        i              = scale * axis_radian.axis.x;
        j              = scale * axis_radian.axis.y;
        k              = scale * axis_radian.axis.z;
    }

    void Quaternion::Set(const EulerAngle& euler_angle)
    {
        Real roll      = euler_angle.x_rotation * .5f;
        Real pitch     = euler_angle.y_rotation * .5f;
        Real yaw       = euler_angle.z_rotation * .5f;
        Real cos_roll  = cosf(roll);
        Real sin_roll  = sinf(roll);
        Real cos_pitch = cosf(pitch);
        Real sin_pitch = sinf(pitch);
        Real cos_yaw   = cosf(yaw);
        Real sin_yaw   = sinf(yaw);
        r              = cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw;
        i              = sin_roll * cos_pitch * cos_yaw - cos_roll * sin_pitch * sin_yaw;
        j              = cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw;
        k              = cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw;
    }

    void Quaternion::Set(const Quaternion& rhs)
    {
        r = rhs.r;
        i = rhs.i;
        j = rhs.j;
        k = rhs.k;
    }

    void Quaternion::SetNormalize()
    {
        Real d = r * r + i * i + j * j + k * k;
        // Check for zero length quaternion, 
        //and use the no-rotation quaternion in that case.
        if (Math::IsZero(d) == true)
        {
            r = 1.0f;
            return;
        }
        Real multiplier = Math::InvSqrt(d);//1.f / sqrtf(d); 
        r *= multiplier;
        i *= multiplier;
        j *= multiplier;
        k *= multiplier;
    }

    void Quaternion::SetClean()
    {
        if (Math::IsZero(r))
            r = 0.0f;
        if (Math::IsZero(i))
            i = 0.0f;
        if (Math::IsZero(j))
            j = 0.0f;
        if (Math::IsZero(k))
            k = 0.0f;
    }

    void Quaternion::SetZero()
    {
        r = i = j = k = 0.0f;
    }

    void Quaternion::SetIdentity()
    {
        r = 1.0f;
        i = j = k = 0.0f;
    }

    void Quaternion::SetConjugate()
    {
        i = -i;
        j = -j;
        k = -k;
    }

    void Quaternion::SetInverse()
    {
        Real norm = r * r + i * i + j * j + k * k;
        if (Math::IsZero(norm))
        {
            //E5_ASSERT(false, "inverse the zero quaternion");
            return;
        }
        Real inverse_norm = 1.0f / norm;
        r                 = inverse_norm * r;
        i                 = -inverse_norm * i;
        j                 = -inverse_norm * j;
        k                 = -inverse_norm * k;
    }

    Real Quaternion::Norm() const
    {
        return sqrtf(r * r + i * i + j * j + k * k);
    }

    Real Quaternion::NormSquared() const
    {
        return (r * r + i * i + j * j + k * k);
    }

    bool Quaternion::IsZero() const
    {
        return Math::IsZero(r * r + i * i + j * j + k * k);
    }

    bool Quaternion::IsUnit() const
    {
        return Math::IsZero(1.0f - (r * r + i * i + j * j + k * k));
    }

    bool Quaternion::IsIdentity() const
    {
        return Math::IsZero(1.0f - r)
                && Math::IsZero(i)
                && Math::IsZero(j)
                && Math::IsZero(k);
    }

    bool Quaternion::IsEqual(const Quaternion& rhs) const
    {
        if (Math::IsEqual(r, rhs.r) == false)
            return false;
        if (Math::IsEqual(i, rhs.i) == false)
            return false;
        if (Math::IsEqual(j, rhs.j) == false)
            return false;
        if (Math::IsEqual(k, rhs.k) == false)
            return false;
        return true;
    }

    bool Quaternion::IsNotEqual(const Quaternion& rhs) const
    {
        if (Math::IsEqual(r, rhs.r) == false)
            return true;
        if (Math::IsEqual(i, rhs.i) == false)
            return true;
        if (Math::IsEqual(j, rhs.j) == false)
            return true;
        if (Math::IsEqual(k, rhs.k) == false)
            return true;
        return false;
    }

    bool Quaternion::IsLostAxis() const
    {
        return Math::IsZero(1.0f - r);
    }

    Vector3 Quaternion::ToVector() const
    {
        return Vector3(i, j, k);
    }

    Matrix33 Quaternion::ToMatrix() const
    {
        Matrix33 result;
        if (IsUnit())
        {
            Real xs        = i + i;
            Real ys        = j + j;
            Real zs        = k + k;
            Real wx        = r * xs;
            Real wy        = r * ys;
            Real wz        = r * zs;
            Real xx        = i * xs;
            Real xy        = i * ys;
            Real xz        = i * zs;
            Real yy        = j * ys;
            Real yz        = j * zs;
            Real zz        = k * zs;
            result.data[0] = 1.0f - (yy + zz);
            result.data[1] = xy - wz;
            result.data[2] = xz + wy;
            result.data[3] = xy + wz;
            result.data[4] = 1.0f - (xx + zz);
            result.data[5] = yz - wx;
            result.data[6] = xz - wy;
            result.data[7] = yz + wx;
            result.data[8] = 1.0f - (xx + yy);
        }
        return result;
    }

    AxisRadian Quaternion::ToAxisRadian() const
    {
        Real    radian = 2.0f * acosf(r);
        Real    length = sqrtf(1.0f - (r * r));
        Vector3 axis   = Math::Vector3::Y_AXIS;
        if (Math::IsZero(length) == false)
        {
            length = 1.0f / length;
            axis.Set(i * length, j * length, k * length);
        }
        return AxisRadian(axis, radian);
    }

    EulerAngle Quaternion::ToEulerAngle() const
    {
        Real roll = atan2f(2.f * (r * i + j * k), 1.f - (2.f * (i * i + j * j)));
        Real pitch;
        Real sin_pitch = 2.f * (r * j - k * i);
        if (fabsf(sin_pitch) >= 1)
            pitch = copysignf(Math::HALF_PI, sin_pitch); // use 90 degrees if out of range
        else
            pitch = asinf(sin_pitch);
        Real yaw = atan2f(2.f * (r * k + i * j), 1.f - (2.f * (j * j + k * k)));
        return EulerAngle(roll, pitch, yaw);
    }

    Quaternion Quaternion::Conjugate() const
    {
        return Quaternion(r, -i, -j, -k);
    }

    Quaternion Quaternion::Inverse() const
    {
        Real norm = r * r + i * i + j * j + k * k;
        // if we're the zero quaternion, just return
        if (Math::IsZero(norm))
            return Quaternion(r, i, j, k);
        Real inverse_norm = 1.0f / norm;
        return Quaternion(inverse_norm * r, -inverse_norm * i, -inverse_norm * j, -inverse_norm * k);
    }

    Real Quaternion::DotProduct(const Quaternion& quaternion) const
    {
        return (r * quaternion.r + i * quaternion.i + j * quaternion.j + k * quaternion.k);
    }

    Vector3 Quaternion::Rotate(const Vector3& vector) const
    {
        if (IsUnit() == true)
        {
            Real v_multiplier = 2.0f * (i * vector.x + j * vector.y + k * vector.z);
            Real c_multiplier = 2.0f * r;
            Real p_multiplier = c_multiplier * r - 1.0f;
            return Vector3(
                           p_multiplier * vector.x + v_multiplier * i + c_multiplier * (j * vector.z - k * vector.y),
                           p_multiplier * vector.y + v_multiplier * j + c_multiplier * (k * vector.x - i * vector.z),
                           p_multiplier * vector.z + v_multiplier * k + c_multiplier * (i * vector.y - j * vector.x));
        }
        //"Rotate non-unit quaternion"
        return RotateVector(vector);
    }

    Vector3 Quaternion::RotateVector(const Vector3& vector) const
    {
        Quaternion inverse = Inverse();
        Quaternion given_vector(0.0f, vector.x, vector.y, vector.z);
        //calculate qpq^-1
        Quaternion result = ((*this) * given_vector) * inverse;
        return Vector3(result.i, result.j, result.k);
    }

    Quaternion Quaternion::Multiply(const Quaternion& rhs) const
    {
        return Quaternion(
                          r * rhs.r - i * rhs.i - j * rhs.j - k * rhs.k,
                          r * rhs.i + i * rhs.r + j * rhs.k - k * rhs.j,
                          r * rhs.j + j * rhs.r + k * rhs.i - i * rhs.k,
                          r * rhs.k + k * rhs.r + i * rhs.j - j * rhs.i);
    }

    void Quaternion::AddRotation(const Vector3& axis, Real radian)
    {
        Quaternion quaternion(AxisRadian(axis, radian));
        *this = quaternion * (*this);
        SetNormalize();
    }

    void Quaternion::AddRotation(const Quaternion& quaternion)
    {
        (*this) = quaternion * (*this);
        SetNormalize();
    }

    void Quaternion::AddRadian(Real radian)
    {
        Real    curr_rad  = 2.0f * acosf(r);
        Real    length    = sqrtf(1.0f - (r * r));
        Vector3 axis      = Math::Vector3::Y_AXIS;
        Real    half_rad  = (curr_rad + radian) * 0.5f;
        Real    sin_theta = sinf(half_rad);
        Real    cos_theta = cosf(half_rad);
        if (Math::IsZero(length) == false)
        {
            length = 1.0f / length;
            axis.Set(i * length, j * length, k * length);
            r = cos_theta;
            i = sin_theta * axis.x;
            j = sin_theta * axis.y;
            k = sin_theta * axis.z;
        }
    }

    void Quaternion::ChangeAxis(const Vector3& axis)
    {
        Real radian      = 2.0f * acosf(r);
        Real length      = sqrtf(1.0f - (r * r));
        Real axis_length = axis.LengthSquared();
        if (Math::IsZero(length) == false && Math::IsZero(axis_length) == false)
        {
            Real half_rad  = (radian) * 0.5f;
            Real sin_theta = sinf(half_rad);
            Real cos_theta = cosf(half_rad);
            axis_length    = 1.0f / axis_length;
            Real scale     = sin_theta / sqrtf(axis_length);
            r              = cos_theta;
            i              = scale * axis.x;
            j              = scale * axis.y;
            k              = scale * axis.z;
        }
    }

    Quaternion Quaternion::operator-() const
    {
        return Quaternion(-r, -i, -j, -k);
    }

    Quaternion& Quaternion::operator=(const Quaternion& rhs)
    {
        if (this != &rhs)
        {
            r = rhs.r;
            i = rhs.i;
            j = rhs.j;
            k = rhs.k;
        }
        return *this;
    }

    bool Quaternion::operator==(const Quaternion& rhs) const
    {
        if (Math::IsEqual(r, rhs.r) == false)
            return false;
        if (Math::IsEqual(i, rhs.i) == false)
            return false;
        if (Math::IsEqual(j, rhs.j) == false)
            return false;
        if (Math::IsEqual(k, rhs.k) == false)
            return false;
        return true;
    }

    bool Quaternion::operator!=(const Quaternion& rhs) const
    {
        if (Math::IsEqual(r, rhs.r) == false)
            return true;
        if (Math::IsEqual(i, rhs.i) == false)
            return true;
        if (Math::IsEqual(j, rhs.j) == false)
            return true;
        if (Math::IsEqual(k, rhs.k) == false)
            return true;
        return false;
    }

    Real Quaternion::operator[](size_t _i) const
    {
        return (&i)[_i];
    }

    Real& Quaternion::operator[](size_t _i)
    {
        return (&i)[_i];
    }

    Quaternion Quaternion::operator*(const Quaternion& rhs) const
    {
        return Quaternion(
                          r * rhs.r - i * rhs.i - j * rhs.j - k * rhs.k,
                          r * rhs.i + i * rhs.r + j * rhs.k - k * rhs.j,
                          r * rhs.j + j * rhs.r + k * rhs.i - i * rhs.k,
                          r * rhs.k + k * rhs.r + i * rhs.j - j * rhs.i);
    }

    Quaternion& Quaternion::operator*=(const Quaternion& rhs)
    {
        auto q = *this;
        r      = q.r * rhs.r - q.i * rhs.i - q.j * rhs.j - q.k * rhs.k;
        i      = q.r * rhs.i + q.i * rhs.r + q.j * rhs.k - q.k * rhs.j;
        j      = q.r * rhs.j + q.j * rhs.r + q.k * rhs.i - q.i * rhs.k;
        k      = q.r * rhs.k + q.k * rhs.r + q.i * rhs.j - q.j * rhs.i;
        return *this;
    }

    Quaternion Quaternion::operator*(const Vector3& vector) const
    {
        return Quaternion(
                          - i * vector.x - j * vector.y - k * vector.z,
                          r * vector.x + j * vector.z - k * vector.y,
                          r * vector.y + k * vector.x - i * vector.z,
                          r * vector.z + i * vector.y - j * vector.x);
    }

    Matrix33 Quaternion::operator*(const Matrix33& matrix) const
    {
        Matrix33 result;
        if (IsUnit())
        {
            result.data[0] = (1.0f - 2.0f * (j * j + k * k)) * matrix.data[0] + 2.0f * (i * j - r * k) * matrix.data[3] + 2.0f * (i * k + r * j) * matrix.data[6];
            result.data[1] = (1.0f - 2.0f * (j * j + k * k)) * matrix.data[1] + 2.0f * (i * j - r * k) * matrix.data[4] + 2.0f * (i * k + r * j) * matrix.data[7];
            result.data[2] = (1.0f - 2.0f * (j * j + k * k)) * matrix.data[2] + 2.0f * (i * j - r * k) * matrix.data[5] + 2.0f * (i * k + r * j) * matrix.data[8];
            result.data[3] = 2.0f * (i * j + r * k) * matrix.data[0] + (1.0f - 2.0f * (i * i + k * k)) * matrix.data[3] + 2.0f * (j * k - r * i) * matrix.data[6];
            result.data[4] = 2.0f * (i * j + r * k) * matrix.data[1] + (1.0f - 2.0f * (i * i + k * k)) * matrix.data[4] + 2.0f * (j * k - r * i) * matrix.data[7];
            result.data[5] = 2.0f * (i * j + r * k) * matrix.data[2] + (1.0f - 2.0f * (i * i + k * k)) * matrix.data[5] + 2.0f * (j * k - r * i) * matrix.data[8];
            result.data[6] = 2.0f * (i * k - r * j) * matrix.data[0] + 2.0f * (j * k + r * i) * matrix.data[3] + (1.0f - 2.0f * (i * i + j * j)) * matrix.data[6];
            result.data[7] = 2.0f * (i * k - r * j) * matrix.data[1] + 2.0f * (j * k + r * i) * matrix.data[4] + (1.0f - 2.0f * (i * i + j * j)) * matrix.data[7];
            result.data[8] = 2.0f * (i * k - r * j) * matrix.data[2] + 2.0f * (j * k + r * i) * matrix.data[5] + (1.0f - 2.0f * (i * i + j * j)) * matrix.data[8];
        }
        return result;
    }

    Quaternion Quaternion::operator+(const Quaternion& rhs) const
    {
        return Quaternion(r + rhs.r, i + rhs.i, j + rhs.j, k + rhs.k);
    }

    Quaternion& Quaternion::operator+=(const Quaternion& rhs)
    {
        r += rhs.r;
        i += rhs.i;
        j += rhs.j;
        k += rhs.k;
        return *this;
    }

    Quaternion Quaternion::operator-(const Quaternion& rhs) const
    {
        return Quaternion(r - rhs.r, i - rhs.i, j - rhs.j, k - rhs.k);
    }

    Quaternion& Quaternion::operator-=(const Quaternion& rhs)
    {
        r -= rhs.r;
        i -= rhs.i;
        j -= rhs.j;
        k -= rhs.k;
        return *this;
    }

    Quaternion Quaternion::operator*(Real scalar) const
    {
        return Quaternion(r * scalar, i * scalar, j * scalar, k * scalar);
    }

    Quaternion& Quaternion::operator*=(Real scalar)
    {
        r *= scalar;
        i *= scalar;
        j *= scalar;
        k *= scalar;
        return *this;
    }

    Quaternion operator*(const Vector3& vector, const Quaternion& quaternion)
    {
        return Quaternion(
                          -vector.x * quaternion.i - vector.y * quaternion.j - vector.z * quaternion.k,
                          +vector.x * quaternion.r + vector.y * quaternion.k - vector.z * quaternion.j,
                          +vector.y * quaternion.r + vector.z * quaternion.i - vector.x * quaternion.k,
                          +vector.z * quaternion.r + vector.x * quaternion.j - vector.y * quaternion.i);
    }

    Matrix33 operator*(const Matrix33& matrix, const Quaternion& quaternion)
    {
        Matrix33 result;
        if (quaternion.IsUnit())
        {
            result.data[0] = matrix.data[0] * (1.0f - 2.0f * (quaternion.j * quaternion.j + quaternion.k * quaternion.k))
                    + 2.0f * matrix.data[1] * (quaternion.i * quaternion.j + quaternion.r * quaternion.k)
                    + 2.0f * matrix.data[2] * (quaternion.i * quaternion.k - quaternion.r * quaternion.j);
            result.data[1] = matrix.data[1] * (1.0f - 2.0f * (quaternion.i * quaternion.i + quaternion.k * quaternion.k))
                    + 2.0f * matrix.data[0] * (quaternion.i * quaternion.j - quaternion.r * quaternion.k)
                    + 2.0f * matrix.data[2] * (quaternion.j * quaternion.k + quaternion.r * quaternion.i);
            result.data[2] = matrix.data[2] * (1.0f - 2.0f * (quaternion.i * quaternion.i + quaternion.j * quaternion.j))
                    + 2.0f * matrix.data[0] * (quaternion.i * quaternion.k + quaternion.r * quaternion.j)
                    + 2.0f * matrix.data[1] * (quaternion.j * quaternion.k - quaternion.r * quaternion.i);
            result.data[3] = matrix.data[3] * (1.0f - 2.0f * (quaternion.j * quaternion.j + quaternion.k * quaternion.k))
                    + 2.0f * matrix.data[4] * (quaternion.i * quaternion.j + quaternion.r * quaternion.k)
                    + 2.0f * matrix.data[5] * (quaternion.i * quaternion.k - quaternion.r * quaternion.j);
            result.data[4] = matrix.data[4] * (1.0f - 2.0f * (quaternion.i * quaternion.i + quaternion.k * quaternion.k))
                    + 2.0f * matrix.data[3] * (quaternion.i * quaternion.j - quaternion.r * quaternion.k)
                    + 2.0f * matrix.data[5] * (quaternion.j * quaternion.k + quaternion.r * quaternion.i);
            result.data[5] = matrix.data[5] * (1.0f - 2.0f * (quaternion.i * quaternion.i + quaternion.j * quaternion.j))
                    + 2.0f * matrix.data[3] * (quaternion.i * quaternion.k + quaternion.r * quaternion.j)
                    + 2.0f * matrix.data[4] * (quaternion.j * quaternion.k - quaternion.r * quaternion.i);;
            result.data[6] = matrix.data[6] * (1.0f - 2.0f * (quaternion.j * quaternion.j + quaternion.k * quaternion.k))
                    + 2.0f * matrix.data[7] * (quaternion.i * quaternion.j + quaternion.r * quaternion.k)
                    + 2.0f * matrix.data[8] * (quaternion.i * quaternion.k - quaternion.r * quaternion.j);
            result.data[7] = matrix.data[7] * (1.0f - 2.0f * (quaternion.i * quaternion.i + quaternion.k * quaternion.k))
                    + 2.0f * matrix.data[6] * (quaternion.i * quaternion.j - quaternion.r * quaternion.k)
                    + 2.0f * matrix.data[8] * (quaternion.j * quaternion.k + quaternion.r * quaternion.i);
            result.data[8] = matrix.data[8] * (1.0f - 2.0f * (quaternion.i * quaternion.i + quaternion.j * quaternion.j))
                    + 2.0f * matrix.data[6] * (quaternion.i * quaternion.k + quaternion.r * quaternion.j)
                    + 2.0f * matrix.data[7] * (quaternion.j * quaternion.k - quaternion.r * quaternion.i);
        }
        return result;
    }

    Quaternion Quaternion::Identity()
    {
        return Quaternion();
    }

    Quaternion Conjugate(const Quaternion& quaternion)
    {
        return quaternion.Conjugate();
    }

    Quaternion Inverse(const Quaternion& quaternion)
    {
        return quaternion.Inverse();
    }

    Real DotProduct(const Quaternion& quat1, const Quaternion& quat2)
    {
        return quat1.DotProduct(quat2);
    }

    Quaternion LinearInterpolation(const Quaternion& start, const Quaternion& end, Real t)
    {
        Real cos_theta = start.DotProduct(end);
        // initialize result
        Quaternion result = t * end;
        // if "angle" between quaternions is less than 90 degrees
        if (cos_theta >= Math::EPSILON)
        {
            // use standard interpolation
            result += (1.0f - t) * start;
        }
        else
        {
            // otherwise, take the shorter path
            result += (t - 1.0f) * start;
        }
        return result;
    }

    Quaternion SphericalLinearInterpolation(const Quaternion& start, const Quaternion& end, Real t)
    {
        // get cosine of "angle" between quaternions
        Real cos_theta = start.DotProduct(end);
        Real start_interp, end_interp;
        // if "angle" between quaternions is less than 90 degrees
        if (cos_theta >= Math::EPSILON)
        {
            // if angle is greater than zero
            if ((1.0f - cos_theta) > Math::EPSILON)
            {
                // use standard slerp
                Real theta         = acosf(cos_theta);
                Real inv_sin_theta = 1.0f / sinf(theta);
                start_interp       = sinf((1.0f - t) * theta) * inv_sin_theta;
                end_interp         = sinf(t * theta) * inv_sin_theta;
            }
                // angle is close to zero
            else
            {
                // use linear interpolation
                start_interp = 1.0f - t;
                end_interp   = t;
            }
        }
            // otherwise, take the shorter route
        else
        {
            // if angle is less than 180 degrees
            if ((1.0f + cos_theta) > Math::EPSILON)
            {
                // use slerp w/negation of start quaternion
                Real theta         = acosf(-cos_theta);
                Real inv_sin_theta = 1.0f / sinf(theta);
                start_interp       = sinf((t - 1.0f) * theta) * inv_sin_theta;
                end_interp         = sinf(t * theta) * inv_sin_theta;
            }
                // angle is close to 180 degrees
            else
            {
                // use lerp w/negation of start quaternion
                start_interp = t - 1.0f;
                end_interp   = t;
            }
        }
        Quaternion result = start_interp * start + end_interp * end;
        return result;
    }

    Quaternion SwingTwistInterpolation(const Quaternion& start, const Quaternion& end, const Vector3& twist_axis, Real t)
    {
        Quaternion delta_rotation = end * start.Inverse();
        Quaternion swing_full;
        Quaternion twist_full;
        Vector3    r = Vector3(delta_rotation.i, delta_rotation.j, delta_rotation.k);
        // singularity: rotation by 180 degree
        if (r.LengthSquared() < Math::EPSILON)
        {
            Vector3 rotated_twist_axis = delta_rotation.Rotate(twist_axis);
            Vector3 swing_axis         = twist_axis.CrossProduct(rotated_twist_axis);
            if (swing_axis.LengthSquared() > Math::EPSILON)
            {
                Real swing_radian = Radian(twist_axis, rotated_twist_axis);
                swing_full        = Quaternion(swing_axis, swing_radian);
            }
            else
            {
                // more singularity: 
                // rotation axis parallel to twist axis
                swing_full = Quaternion::Identity(); // no swing
            }
            // always twist 180 degree on singularity
            twist_full = Quaternion(twist_axis, Math::PI);
        }
        else
        {
            // meat of swing-twist decomposition
            Vector3 p  = r.ProjectionTo(twist_axis);
            twist_full = Quaternion(delta_rotation.r, p.x, p.y, p.z);
            twist_full.SetNormalize();
            swing_full = delta_rotation * twist_full.Inverse();
        }
        Quaternion swing = SphericalLinearInterpolation(Quaternion::Identity(), swing_full, t);
        Quaternion twist = SphericalLinearInterpolation(Quaternion::Identity(), twist_full, t);
        return twist * swing;
    }

    Quaternion operator*(Real scalar, const Quaternion& quaternion)
    {
        return Quaternion(scalar * quaternion.r, scalar * quaternion.i, scalar * quaternion.j, scalar * quaternion.k);
    }

    std::ostream& operator<<(std::ostream& os, const Quaternion& rhs)
    {
        if (rhs.IsUnit() == true)
        {
            Real    radian = 2.0f * acosf(rhs.r);
            Real    length = sqrtf(1.0f - (rhs.r * rhs.r));
            Vector3 axis   = Math::Vector3::Y_AXIS;
            if (Math::IsZero(length) == false)
            {
                length = 1.0f / length;
                axis.Set(rhs.i * length, rhs.j * length, rhs.k * length);
            }
            Real degree = Math::RadiansToDegrees(radian);
            //os << std::setprecision(3);
            os << "[cos(" << degree << ") + sin(" << degree << ") * (" << axis.x << "i + " << axis.y << "j + " << axis.z << "k)]";
            //os << std::setprecision(std::ios_base::precision);
        }
        else
        {
            os << "[" << rhs.r << ", " << rhs.i << ", " << rhs.j << ", " << rhs.k << "]";
        }
        return os;
    }

    std::wstringstream& operator<<(std::wstringstream& os, const Quaternion& rhs)
    {
        if (rhs.IsUnit() == true)
        {
            Real    radian = 2.0f * acosf(rhs.r);
            Real    length = sqrtf(1.0f - (rhs.r * rhs.r));
            Vector3 axis   = Math::Vector3::Y_AXIS;
            if (Math::IsZero(length) == false)
            {
                length = 1.0f / length;
                axis.Set(rhs.i * length, rhs.j * length, rhs.k * length);
            }
            Real degree = Math::RadiansToDegrees(radian);
            //os << std::setprecision(3);
            os << "[cos(" << degree << ") + sin(" << degree << ") * (" << axis.x << "i + " << axis.y << "j + " << axis.z << "k)]";
            //os << std::setprecision(std::ios_base::precision);
        }
        else
        {
            os << "[" << rhs.r << ", " << rhs.i << ", " << rhs.j << ", " << rhs.k << "]";
        }
        return os;
    }
}
