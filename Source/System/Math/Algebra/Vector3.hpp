
#pragma once
#include "../Utility/MathDef.hpp"

namespace CS460
{
    class Vector4;
    class Vector2;
    class Matrix33;

    class Vector3
    {
    public:
        explicit Vector3(Real x = 0.f, Real y = 0.f, Real z = 0.f);
        explicit Vector3(Real arr[3]);
        explicit Vector3(const Vector2& rhs);
        explicit Vector3(const Vector4& rhs);
        Vector3(const Vector3& rhs);

        ~Vector3();

        void Set(Real x = 0.0f, Real y = 0.0f, Real z = 0.0f);
        void SetZero();
        void SetInverse();
        void SetNegate();
        void SetNormalize();
        void SetHalf();
        void SetClean();
        void SetProjection(const Vector3& a, const Vector3& b);

        Real Length() const;
        Real LengthSquared() const;
        Real Smallest() const;
        Real Largest() const;
        Real* GetData();

        Real    DistanceTo(const Vector3& rhs) const;
        Real    DistanceSquaredTo(const Vector3& rhs) const;
        Vector3 ProjectionTo(const Vector3& rhs) const;
        Vector3 ProjectionFrom(const Vector3& rhs) const;
        Vector3 Normalize() const;
        Vector3 Unit() const;
        Vector3 Half() const;
        Vector3 Inverse() const;
        Vector3 Negate() const;
        Vector3 Scale(Real scale) const;
        Vector3 Absolute() const;

        Real     DotProduct(const Vector3& rhs) const;
        Vector3  CrossProduct(const Vector3& rhs) const;
        Matrix33 OuterProduct(const Vector3& rhs) const;
        Vector3  HadamardProduct(const Vector3& rhs) const;
        Vector3  CrossProductTwice(const Vector3& rhs) const;

        bool IsValid() const;
        bool IsZero() const;
        bool IsEqual(const Vector3& rhs) const;
        bool IsNotEqual(const Vector3& rhs) const;

        Real GrepVec1(size_t flag0) const;
        Vector2 GrepVec2(size_t flag0, size_t flag1) const;
        Vector3 GrepVec3(size_t flag0, size_t flag1, size_t flag2) const;
        Vector4 GrepVec4(size_t flag0, size_t flag1, size_t flag2, size_t flag3) const;
        size_t SafeFlag(size_t given) const;

    public:
        friend Real     DotProduct(const Vector3& vec1, const Vector3& vec2);
        friend Vector3  CrossProduct(const Vector3& vec1, const Vector3& vec2);
        friend Matrix33 OuterProduct(const Vector3& vec1, const Vector3& vec2);
        friend Vector3  HadamardProduct(const Vector3& vec1, const Vector3& vec2);
        friend Vector3  CrossProductTwice(const Vector3& vec1, const Vector3& vec2);
        friend Vector3  Projection(const Vector3& vec1, const Vector3& vec2);
        friend Vector3  LinearInterpolation(const Vector3& start, const Vector3& end, Real t);
        friend Real     Radian(const Vector3& a, const Vector3& b);

    public:
        bool                 operator ==(const Vector3& rhs) const;
        bool                 operator !=(const Vector3& rhs) const;
        Vector3              operator -() const;
        Vector3&             operator =(const Vector2& rhs);
        Vector3&             operator =(const Vector3& rhs);
        Vector3&             operator =(const Vector4& rhs);
        Vector3&             operator =(Real rhs);
        Vector3&             operator +=(const Vector3& rhs);
        Vector3&             operator +=(Real real);
        Vector3&             operator -=(const Vector3& rhs);
        Vector3&             operator -=(Real real);
        Vector3&             operator *=(Real real);
        Vector3&             operator /=(Real real);
        Vector3              operator +(const Vector3& rhs) const;
        Vector3              operator +(Real real) const;
        Vector3              operator -(const Vector3& rhs) const;
        Vector3              operator -(Real real) const;
        Vector3              operator *(Real real) const;
        Vector3              operator /(Real real) const;
        friend std::ostream& operator<<(std::ostream& os, const Vector3& rhs);
        friend std::wstringstream& operator<<(std::wstringstream& os, const Vector3& rhs);
        friend Vector3       operator*(Real real, const Vector3& vector);
        Vector3&             operator ++();
        Vector3              operator ++(int);
        Vector3&             operator --();
        Vector3              operator --(int);
        Real                 operator[](size_t i) const;
        Real&                operator[](size_t i);
        Real                 operator *(const Vector3& rhs) const;

    public:
        Real x, y, z;
    };
}
