
#include "Vector4.hpp"
#include "..//Utility/Utility.hpp"
#include <ostream>
#include "Matrix44.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "../Utility/VectorDef.hpp"

namespace CS460
{
    Vector4::Vector4(Real x, Real y, Real z, Real w)
        : x(x), y(y), z(z), w(w)
    {
    }

    Vector4::Vector4(Real arr[4])
        : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3])
    {
    }

    Vector4::Vector4(const Vector2& rhs)
        : x(rhs.x), y(rhs.y), z(0.0f), w(0.0f)
    {
    }

    Vector4::Vector4(const Vector3& rhs)
        : x(rhs.x), y(rhs.y), z(rhs.z), w(0.0f)
    {
    }

    Vector4::Vector4(const Vector3& rhs, Real w)
        : x(rhs.x), y(rhs.y), z(rhs.z), w(w)
    {
    }

    Vector4::Vector4(const Vector4& rhs)
        : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
    {
    }

    Vector4::~Vector4()
    {
    }

    void Vector4::Set(Real _x, Real _y, Real _z, Real _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    void Vector4::SetZero()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }

    void Vector4::SetInverse()
    {
        this->x = Math::IsZero(x) ? 0.0f : 1.0f / this->x;
        this->y = Math::IsZero(y) ? 0.0f : 1.0f / this->y;
        this->z = Math::IsZero(z) ? 0.0f : 1.0f / this->z;
        this->w = Math::IsZero(w) ? 0.0f : 1.0f / this->w;
    }

    void Vector4::SetNegate()
    {
        x = -x;
        y = -y;
        z = -z;
        w = -w;
    }

    void Vector4::SetNormalize()
    {
        Real length = sqrt(x * x + y * y + z * z + w * w);
        if (length > 0.0f)
        {
            (*this) *= (1.0f / length);
        }
    }

    void Vector4::SetHalf()
    {
        this->x *= 0.5f;
        this->y *= 0.5f;
        this->z *= 0.5f;
        this->w *= 0.5f;
    }

    void Vector4::SetClean()
    {
        if (Math::IsZero(x))
            x = 0.0f;
        if (Math::IsZero(y))
            y = 0.0f;
        if (Math::IsZero(z))
            z = 0.0f;
        if (Math::IsZero(w))
            w = 0.0f;
    }

    void Vector4::SetProjection(const Vector4& a, const Vector4& b)
    {
        Real multiplier = (a.DotProduct(b)) / (b.DotProduct(b));
        this->x         = b.x * multiplier;
        this->y         = b.y * multiplier;
        this->z         = b.z * multiplier;
        this->w         = b.w * multiplier;
    }

    Real Vector4::Length() const
    {
        return sqrtf(x * x + y * y + z * z + w * w);
    }

    Real Vector4::LengthSquared() const
    {
        return (x * x + y * y + z * z + w * w);
    }

    Real Vector4::DistanceTo(const Vector4& rhs) const
    {
        Real _x = rhs.x - this->x;
        Real _y = rhs.y - this->y;
        Real _z = rhs.z - this->z;
        Real _w = rhs.w - this->w;
        return sqrtf(_x * _x + _y * _y + _z * _z + _w * _w);
    }

    Real Vector4::DistanceSquaredTo(const Vector4& rhs) const
    {
        Real _x = rhs.x - this->x;
        Real _y = rhs.y - this->y;
        Real _z = rhs.z - this->z;
        Real _w = rhs.w - this->w;
        return (_x * _x + _y * _y + _z * _z + _w * _w);
    }

    Vector4 Vector4::ProjectionTo(const Vector4& rhs) const
    {
        Vector4    result     = rhs;
        const Real multiplier = ((*this).DotProduct(rhs)) / (rhs.DotProduct(rhs));
        result *= multiplier;
        return result;
    }

    Vector4 Vector4::ProjectionFrom(const Vector4& rhs) const
    {
        Vector4 result     = *this;
        Real    multiplier = (rhs.DotProduct(*this)) / ((*this).DotProduct(*this));
        result *= multiplier;
        return result;
    }

    Vector4 Vector4::Unit() const
    {
        Vector4 result = *this;
        result.SetNormalize();
        return result;
    }

    Vector4 Vector4::Half() const
    {
        Vector4 result = *this;
        result.SetHalf();
        return result;
    }

    Real Vector4::DotProduct(const Vector4& rhs) const
    {
        return (x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w);
    }

    Vector4 Vector4::CrossProduct(const Vector4& rhs) const
    {
        return Vector4(
                       y * rhs.z - z * rhs.y,
                       -x * rhs.z + z * rhs.x,
                       x * rhs.y - y * rhs.x,
                       1.0f);
    }

    Matrix44 Vector4::OuterProduct(const Vector4& rhs) const
    {
        return Matrix44(
                        (this->x * rhs.x), (this->x * rhs.y), (this->x * rhs.z), (this->x * rhs.w),
                        (this->y * rhs.x), (this->y * rhs.y), (this->y * rhs.z), (this->y * rhs.w),
                        (this->z * rhs.x), (this->z * rhs.y), (this->z * rhs.z), (this->z * rhs.w),
                        (this->w * rhs.x), (this->w * rhs.y), (this->w * rhs.z), (this->w * rhs.w)
                       );
    }

    Vector4 Vector4::HadamardProduct(const Vector4& rhs) const
    {
        return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }

    bool Vector4::IsValid() const
    {
        return Math::IsValid(x) && Math::IsValid(y) && Math::IsValid(z) && Math::IsValid(w);
    }

    bool Vector4::IsZero() const
    {
        return Math::IsZero(x) && Math::IsZero(y) && Math::IsZero(z) && Math::IsZero(w);
    }

    bool Vector4::IsEqual(const Vector4& rhs) const
    {
        return false;
    }

    bool Vector4::IsNotEqual(const Vector4& rhs) const
    {
        return false;
    }

    Real Vector4::GrepVec1(size_t flag0) const
    {
        return (*this)[flag0];
    }

    Vector2 Vector4::GrepVec2(size_t flag0, size_t flag1) const
    {
        return Vector2((*this)[SafeFlag(flag0)], (*this)[SafeFlag(flag1)]);
    }

    Vector3 Vector4::GrepVec3(size_t flag0, size_t flag1, size_t flag2) const
    {
        return Vector3((*this)[SafeFlag(flag0)], (*this)[SafeFlag(flag1)], (*this)[SafeFlag(flag2)]);
    }

    Vector4 Vector4::GrepVec4(size_t flag0, size_t flag1, size_t flag2, size_t flag3) const
    {
        return Vector4((*this)[SafeFlag(flag0)], (*this)[SafeFlag(flag1)], (*this)[SafeFlag(flag2)], (*this)[SafeFlag(flag3)]);
    }

    size_t Vector4::SafeFlag(size_t given) const
    {
        return given > Math::Vector::W ? Math::Vector::W : given;
    }

    bool Vector4::operator==(const Vector4& rhs) const
    {
        return (Math::IsEqual(x, rhs.x)
            && Math::IsEqual(y, rhs.y)
            && Math::IsEqual(z, rhs.z)
            && Math::IsEqual(z, rhs.w));
    }

    bool Vector4::operator!=(const Vector4& rhs) const
    {
        return (Math::IsNotEqual(x, rhs.x)
            || Math::IsNotEqual(y, rhs.y)
            || Math::IsNotEqual(z, rhs.z)
            || Math::IsNotEqual(z, rhs.w));
    }

    Vector4 Vector4::operator-() const
    {
        return Vector4(-x, -y, -z, -w);
    }

    Vector4& Vector4::operator=(const Vector2& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector4& Vector4::operator=(const Vector3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    Vector4& Vector4::operator=(const Vector4& rhs)
    {
        if (this != &rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;
        }
        return *this;
    }

    Vector4& Vector4::operator=(Real rhs)
    {
        x = rhs;
        y = rhs;
        z = rhs;
        w = rhs;
        return *this;
    }

    Vector4& Vector4::operator+=(const Vector4& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vector4& Vector4::operator+=(Real real)
    {
        x += real;
        y += real;
        z += real;
        w += real;
        return *this;
    }

    Vector4& Vector4::operator-=(const Vector4& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Vector4& Vector4::operator-=(Real real)
    {
        x -= real;
        y -= real;
        z -= real;
        w -= real;
        return *this;
    }

    Vector4& Vector4::operator*=(Real real)
    {
        x *= real;
        y *= real;
        z *= real;
        w *= real;
        return *this;
    }

    Vector4& Vector4::operator/=(Real real)
    {
        x /= real;
        y /= real;
        z /= real;
        w /= real;
        return *this;
    }

    Vector4 Vector4::operator+(const Vector4& rhs) const
    {
        return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    Vector4 Vector4::operator+(Real real) const
    {
        return Vector4(x + real, y + real, z + real, w + real);
    }

    Vector4 Vector4::operator-(const Vector4& rhs) const
    {
        return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    Vector4 Vector4::operator-(Real real) const
    {
        return Vector4(x - real, y - real, z - real, w - real);
    }

    Vector4 Vector4::operator*(Real real) const
    {
        return Vector4(x * real, y * real, z * real, w * real);
    }

    Vector4 Vector4::operator/(Real real) const
    {
        return Vector4(x / real, y / real, z / real, w / real);
    }

    Vector4& Vector4::operator++()
    {
        ++x;
        ++y;
        ++z;
        ++w;
        return *this;
    }

    Vector4 Vector4::operator++(int)
    {
        Vector4 result(*this);
        ++(*this);
        return result;
    }

    Vector4& Vector4::operator--()
    {
        --x;
        --y;
        --z;
        --w;
        return *this;
    }

    Vector4 Vector4::operator--(int)
    {
        Vector4 result(*this);
        --(*this);
        return result;
    }

    Real Vector4::operator[](size_t i) const
    {
        return (&x)[i];
    }

    Real& Vector4::operator[](size_t i)
    {
        return (&x)[i];
    }

    Real DotProduct(const Vector4& vec1, const Vector4& vec2)
    {
        return vec1.DotProduct(vec2);
    }

    Vector4 CrossProduct(const Vector4& vec1, const Vector4& vec2)
    {
        return vec1.CrossProduct(vec2);
    }

    Matrix44 OuterProduct(const Vector4& vec1, const Vector4& vec2)
    {
        return vec1.OuterProduct(vec2);
    }

    Vector4 HadamardProduct(const Vector4& vec1, const Vector4& vec2)
    {
        return vec1.HadamardProduct(vec2);
    }

    Vector4 Projection(const Vector4& vec1, const Vector4& vec2)
    {
        return vec1.ProjectionTo(vec2);
    }

    Vector4 LinearInterpolation(const Vector4& start, const Vector4& end, Real t)
    {
        Vector4 result;
        result.x = (1.0f - t) * start.x + t * end.x;
        result.y = (1.0f - t) * start.y + t * end.y;
        result.z = (1.0f - t) * start.z + t * end.z;
        result.w = (1.0f - t) * start.w + t * end.w;
        return result;
    }

    std::ostream& operator<<(std::ostream& os, const Vector4& rhs)
    {
        os << "[" << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << "]";
        return os;
    }

    Vector4 operator*(Real real, const Vector4& vector)
    {
        return Vector4(vector.x * real, vector.y * real, vector.z * real, vector.w * real);
    }
}
