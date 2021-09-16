#pragma once
#include "../Utility/MathDef.hpp"

namespace CS460
{
    class Vector2;
    class Vector3;
    class Matrix44;

    class Vector4
    {
    public:
        explicit Vector4(Real x = 0.0f, Real y = 0.0f, Real z = 0.0f, Real w = 0.0f);
        explicit Vector4(Real arr[4]);
        explicit Vector4(const Vector2& rhs);
        explicit Vector4(const Vector3& rhs);
        explicit Vector4(const Vector3& rhs, Real w);
        Vector4(const Vector4& rhs);
        ~Vector4();

        void Set(Real x = 0.0f, Real y = 0.0f, Real z = 0.0f, Real w = 0.0f);
        void SetZero();
        void SetInverse();
        void SetNegate();
        void SetNormalize();
        void SetHalf();
        void SetClean();
        void SetProjection(const Vector4& a, const Vector4& b);

        Real    Length() const;
        Real    LengthSquared() const;
        Real    DistanceTo(const Vector4& rhs) const;
        Real    DistanceSquaredTo(const Vector4& rhs) const;
        Vector4 ProjectionTo(const Vector4& rhs) const;
        Vector4 ProjectionFrom(const Vector4& rhs) const;
        Vector4 Unit() const;
        Vector4 Half() const;

        Real     DotProduct(const Vector4& rhs) const;
        Vector4  CrossProduct(const Vector4& rhs) const;
        Matrix44 OuterProduct(const Vector4& rhs) const;
        Vector4  HadamardProduct(const Vector4& rhs) const;

        bool IsValid() const;
        bool IsZero() const;
        bool IsEqual(const Vector4& rhs) const;
        bool IsNotEqual(const Vector4& rhs) const;

        Real    GrepVec1(size_t flag0) const;
        Vector2 GrepVec2(size_t flag0, size_t flag1) const;
        Vector3 GrepVec3(size_t flag0, size_t flag1, size_t flag2) const;
        Vector4 GrepVec4(size_t flag0, size_t flag1, size_t flag2, size_t flag3) const;
        size_t  SafeFlag(size_t given) const;

    public:
        friend Real     DotProduct(const Vector4& vec1, const Vector4& vec2);
        friend Vector4  CrossProduct(const Vector4& vec1, const Vector4& vec2);
        friend Matrix44 OuterProduct(const Vector4& vec1, const Vector4& vec2);
        friend Vector4  HadamardProduct(const Vector4& vec1, const Vector4& vec2);
        friend Vector4  Projection(const Vector4& vec1, const Vector4& vec2);
        friend Vector4  LinearInterpolation(const Vector4& start, const Vector4& end, Real t);

    public:
        bool                 operator ==(const Vector4& rhs) const;
        bool                 operator !=(const Vector4& rhs) const;
        Vector4              operator -() const;
        Vector4&             operator =(const Vector2& rhs);
        Vector4&             operator =(const Vector3& rhs);
        Vector4&             operator =(const Vector4& rhs);
        Vector4&             operator =(Real rhs);
        Vector4&             operator +=(const Vector4& rhs);
        Vector4&             operator +=(Real real);
        Vector4&             operator -=(const Vector4& rhs);
        Vector4&             operator -=(Real real);
        Vector4&             operator *=(Real real);
        Vector4&             operator /=(Real real);
        Vector4              operator +(const Vector4& rhs) const;
        Vector4              operator +(Real real) const;
        Vector4              operator -(const Vector4& rhs) const;
        Vector4              operator -(Real real) const;
        Vector4              operator *(Real real) const;
        Vector4              operator /(Real real) const;
        friend std::ostream& operator <<(std::ostream& os, const Vector4& rhs);
        friend Vector4       operator *(Real real, const Vector4& vector);
        Vector4&             operator ++();
        Vector4              operator ++(int);
        Vector4&             operator --();
        Vector4              operator --(int);
        Real                 operator[](size_t i) const;
        Real&                operator[](size_t i);

    public:
        Real x, y, z, w;
    };
}
