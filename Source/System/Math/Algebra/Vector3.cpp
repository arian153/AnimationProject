#include "Vector3.hpp"
#include "Matrix33.hpp"
#include "..//Utility/Utility.hpp"
#include <ostream>
#include "Vector4.hpp"
#include "Vector2.hpp"
#include <sstream>

namespace CS460
{
    Vector3::Vector3(Real x, Real y, Real z)
        : x(x), y(y), z(z)
    {
    }

    Vector3::Vector3(Real arr[3])
        : x(arr[0]), y(arr[1]), z(arr[2])
    {
    }

    Vector3::Vector3(const Vector2& rhs)
        : x(rhs.x), y(rhs.y), z(0.0f)
    {
    }

    Vector3::Vector3(const Vector4& rhs)
        : x(rhs.x), y(rhs.y), z(rhs.z)
    {
    }

    Vector3::Vector3(const Vector3& rhs)
        : x(rhs.x), y(rhs.y), z(rhs.z)
    {
    }

    Vector3::~Vector3()
    {
    }

    void Vector3::Set(Real _x, Real _y, Real _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void Vector3::SetZero()
    {
        x = 0.f;
        y = 0.f;
        z = 0.f;
    }

    void Vector3::SetInverse()
    {
        x = Math::IsZero(x) ? 0.0f : 1.0f / x;
        y = Math::IsZero(y) ? 0.0f : 1.0f / y;
        z = Math::IsZero(z) ? 0.0f : 1.0f / z;
    }

    void Vector3::SetNegate()
    {
        x = - x;
        y = - y;
        z = - z;
    }

    void Vector3::SetNormalize()
    {
        Real length = sqrtf(x * x + y * y + z * z);
        if (length > 0.f)
        {
            *this *= 1.f / length;
        }
    }

    void Vector3::SetHalf()
    {
        x *= 0.5f;
        y *= 0.5f;
        z *= 0.5f;
    }

    void Vector3::SetClean()
    {
        if (Math::IsZero(x))
            x = 0.0f;
        if (Math::IsZero(y))
            y = 0.0f;
        if (Math::IsZero(z))
            z = 0.0f;
    }

    void Vector3::SetProjection(const Vector3& a, const Vector3& b)
    {
        Real multiplier = a.DotProduct(b) / b.DotProduct(b);
        x               = b.x * multiplier;
        y               = b.y * multiplier;
        z               = b.z * multiplier;
    }

    Real Vector3::Length() const
    {
        return sqrtf(x * x + y * y + z * z);
    }

    Real Vector3::LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    Real Vector3::Smallest() const
    {
        Real result = x < y ? x : y;
        return result < z ? result : z;
    }

    Real Vector3::Largest() const
    {
        Real result = x > y ? x : y;
        return result > z ? result : z;
    }

    Real* Vector3::GetData()
    {
        return &x;
    }

    Real Vector3::DistanceTo(const Vector3& rhs) const
    {
        Real _x = rhs.x - x;
        Real _y = rhs.y - y;
        Real _z = rhs.z - z;
        return sqrtf(_x * _x + _y * _y + _z * _z);
    }

    Real Vector3::DistanceSquaredTo(const Vector3& rhs) const
    {
        Real _x = rhs.x - x;
        Real _y = rhs.y - y;
        Real _z = rhs.z - z;
        return _x * _x + _y * _y + _z * _z;
    }

    Vector3 Vector3::ProjectionTo(const Vector3& rhs) const
    {
        Vector3 result     = rhs;
        Real    multiplier = (*this).DotProduct(rhs) / rhs.DotProduct(rhs);
        result *= multiplier;
        return result;
    }

    Vector3 Vector3::ProjectionFrom(const Vector3& rhs) const
    {
        Vector3 result     = *this;
        Real    multiplier = rhs.DotProduct(*this) / (*this).DotProduct(*this);
        result *= multiplier;
        return result;
    }

    Vector3 Vector3::Normalize() const
    {
        Vector3 result = *this;
        result.SetNormalize();
        return result;
    }

    Vector3 Vector3::Unit() const
    {
        Real length = sqrtf(x * x + y * y + z * z);
        length      = length > 0.0f ? 1.0f / length : 0.0f;
        return Vector3(x * length, y * length, z * length);
    }

    Vector3 Vector3::Half() const
    {
        Vector3 result = *this;
        result.SetHalf();
        return result;
    }

    Vector3 Vector3::Inverse() const
    {
        return Vector3(
                       Math::IsZero(x) ? 0.0f : 1.0f / x,
                       Math::IsZero(y) ? 0.0f : 1.0f / y,
                       Math::IsZero(z) ? 0.0f : 1.0f / z);
    }

    Vector3 Vector3::Negate() const
    {
        return Vector3(- x, - y, - z);
    }

    Vector3 Vector3::Scale(Real scale) const
    {
        return Vector3(x * scale, y * scale, z * scale);
    }

    Vector3 Vector3::Absolute() const
    {
        return Vector3(fabsf(x), fabsf(y), fabsf(z));
    }

    Matrix33 Vector3::SkewSymmetricMatrix() const
    {
        //get skew-symmetric matrix to represent "CrossProduct(A, r) = TildeA * r"
        //given r, A are both vector3
        return Matrix33(
                        0.0f, -z, y,
                        z, 0.0f, -x,
                        -y, x, 0.0f);
    }

    Matrix33 Vector3::ProjectionMatrix() const
    {
        // get projection matrix to represent "DotProduct(r, A)*A = HatA * r"
        // given r, A are both vector3
        return Matrix33(
                        x * x, x * y, x * z,
                        y * x, y * y, y * z,
                        z * x, z * y, z * z);
    }

    Real Vector3::DotProduct(const Vector3& rhs) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    Vector3 Vector3::CrossProduct(const Vector3& rhs) const
    {
        return Vector3(
                       y * rhs.z - z * rhs.y,
                       -x * rhs.z + z * rhs.x,
                       x * rhs.y - y * rhs.x);
    }

    Matrix33 Vector3::OuterProduct(const Vector3& rhs) const
    {
        return Matrix33(
                        x * rhs.x, x * rhs.y, x * rhs.z,
                        y * rhs.x, y * rhs.y, y * rhs.z,
                        z * rhs.x, z * rhs.y, z * rhs.z
                       );
    }

    Vector3 Vector3::HadamardProduct(const Vector3& rhs) const
    {
        return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    Vector3 Vector3::CrossProductTwice(const Vector3& rhs) const
    {
        return CrossProduct(rhs).CrossProduct(*this);
    }

    bool Vector3::IsValid() const
    {
        return Math::IsValid(x) && Math::IsValid(y) && Math::IsValid(z);
    }

    bool Vector3::IsZero() const
    {
        return Math::IsZero(x) && Math::IsZero(y) && Math::IsZero(z);
    }

    bool Vector3::IsEqual(const Vector3& rhs) const
    {
        return Math::IsEqual(x, rhs.x)
                && Math::IsEqual(y, rhs.y)
                && Math::IsEqual(z, rhs.z);
    }

    bool Vector3::IsNotEqual(const Vector3& rhs) const
    {
        return Math::IsNotEqual(x, rhs.x)
                || Math::IsNotEqual(y, rhs.y)
                || Math::IsNotEqual(z, rhs.z);
    }

    Real Vector3::GrepVec1(size_t flag0) const
    {
        return (*this)[SafeFlag(flag0)];
    }

    Vector2 Vector3::GrepVec2(size_t flag0, size_t flag1) const
    {
        return Vector2((*this)[SafeFlag(flag0)], (*this)[SafeFlag(flag1)]);
    }

    Vector3 Vector3::GrepVec3(size_t flag0, size_t flag1, size_t flag2) const
    {
        return Vector3((*this)[SafeFlag(flag0)], (*this)[SafeFlag(flag1)], (*this)[SafeFlag(flag2)]);
    }

    Vector4 Vector3::GrepVec4(size_t flag0, size_t flag1, size_t flag2, size_t flag3) const
    {
        return Vector4((*this)[SafeFlag(flag0)], (*this)[SafeFlag(flag1)], (*this)[SafeFlag(flag2)], (*this)[SafeFlag(flag3)]);
    }

    size_t Vector3::SafeFlag(size_t given) const
    {
        return given > Math::Vector::Z ? Math::Vector::Z : given;
    }

    bool Vector3::operator==(const Vector3& rhs) const
    {
        return Math::IsEqual(x, rhs.x)
                && Math::IsEqual(y, rhs.y)
                && Math::IsEqual(z, rhs.z);
    }

    bool Vector3::operator!=(const Vector3& rhs) const
    {
        return Math::IsNotEqual(x, rhs.x)
                || Math::IsNotEqual(y, rhs.y)
                || Math::IsNotEqual(z, rhs.z);
    }

    Vector3 Vector3::operator-() const
    {
        return Vector3(-x, -y, -z);
    }

    Vector3& Vector3::operator=(const Vector2& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector3& Vector3::operator=(const Vector3& rhs)
    {
        if (this != &rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
        }
        return *this;
    }

    Vector3& Vector3::operator=(const Vector4& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    Vector3& Vector3::operator=(Real rhs)
    {
        x = rhs;
        y = rhs;
        z = rhs;
        return *this;
    }

    Vector3& Vector3::operator+=(const Vector3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vector3& Vector3::operator+=(Real real)
    {
        x += real;
        y += real;
        z += real;
        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vector3& Vector3::operator-=(Real real)
    {
        x -= real;
        y -= real;
        z -= real;
        return *this;
    }

    Vector3& Vector3::operator*=(Real real)
    {
        x *= real;
        y *= real;
        z *= real;
        return *this;
    }

    Vector3& Vector3::operator/=(Real real)
    {
        x /= real;
        y /= real;
        z /= real;
        return *this;
    }

    Vector3 Vector3::operator+(const Vector3& rhs) const
    {
        return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Vector3 Vector3::operator+(Real real) const
    {
        return Vector3(x + real, y + real, z + real);
    }

    Vector3 Vector3::operator-(const Vector3& rhs) const
    {
        return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Vector3 Vector3::operator-(Real real) const
    {
        return Vector3(x - real, y - real, z - real);
    }

    Vector3 Vector3::operator*(Real real) const
    {
        return Vector3(x * real, y * real, z * real);
    }

    Vector3 Vector3::operator/(Real real) const
    {
        return Vector3(x / real, y / real, z / real);
    }

    std::ostream& operator<<(std::ostream& os, const Vector3& rhs)
    {
        os << "[" << rhs.x << ", " << rhs.y << ", " << rhs.z << "]";
        return os;
    }

    std::wstringstream& operator<<(std::wstringstream& os, const Vector3& rhs)
    {
        os << "[" << rhs.x << ", " << rhs.y << ", " << rhs.z << "]";
        return os;
    }

    Vector3 operator*(Real real, const Vector3& vector)
    {
        return Vector3(vector.x * real, vector.y * real, vector.z * real);
    }

    Vector3& Vector3::operator++()
    {
        ++x;
        ++y;
        ++z;
        return *this;
    }

    Vector3 Vector3::operator++(int)
    {
        Vector3 result(*this);
        ++*this;
        return result;
    }

    Vector3& Vector3::operator--()
    {
        --x;
        --y;
        --z;
        return *this;
    }

    Vector3 Vector3::operator--(int)
    {
        Vector3 result(*this);
        --*this;
        return result;
    }

    Real Vector3::operator[](size_t i) const
    {
        return (&x)[i];
    }

    Real& Vector3::operator[](size_t i)
    {
        return (&x)[i];
    }

    Real Vector3::operator*(const Vector3& rhs) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    Real DotProduct(const Vector3& vec1, const Vector3& vec2)
    {
        return vec1.DotProduct(vec2);
    }

    Vector3 CrossProduct(const Vector3& vec1, const Vector3& vec2)
    {
        return vec1.CrossProduct(vec2);
    }

    Matrix33 OuterProduct(const Vector3& vec1, const Vector3& vec2)
    {
        return vec1.OuterProduct(vec2);
    }

    Vector3 HadamardProduct(const Vector3& vec1, const Vector3& vec2)
    {
        return vec1.HadamardProduct(vec2);
    }

    Vector3 CrossProductTwice(const Vector3& vec1, const Vector3& vec2)
    {
        return vec1.CrossProductTwice(vec2);
    }

    Vector3 Projection(const Vector3& vec1, const Vector3& vec2)
    {
        return vec1.ProjectionTo(vec2);
    }

    Vector3 LinearInterpolation(const Vector3& start, const Vector3& end, Real t)
    {
        Vector3 result;
        result.x = (1.0f - t) * start.x + t * end.x;
        result.y = (1.0f - t) * start.y + t * end.y;
        result.z = (1.0f - t) * start.z + t * end.z;
        return result;
    }

    Real BetweenAngle(const Vector3& a, const Vector3& b)
    {
        Real cos_theta = DotProduct(a.Normalize(), b.Normalize());
        if (fabsf(cos_theta) > 1.0f)
        {
            return 0.0f;
        }
        return acosf(cos_theta);
    }
}
