
#pragma once
#include "../Utility/MathDef.hpp"

namespace CS460
{
    class Vector4;
    class Vector3;
    class Matrix22;

    class Vector2
    {
    public:
        explicit Vector2(Real x = 0.0f, Real y = 0.0f);
        explicit Vector2(Real arr[2]);
        explicit Vector2(const Vector3& rhs);
        explicit Vector2(const Vector4& rhs);
        Vector2(const Vector2& rhs);
        ~Vector2();

        void Set(Real x = 0.0f, Real y = 0.0f);
        void SetZero();
        void SetInverse();
        void SetNegate();
        void SetNormalize();
        void SetHalf();
        void SetClean();
        void SetProjection(const Vector2& a, const Vector2& b);

        Real    Length() const;
        Real    LengthSquared() const;
        Real    DistanceTo(const Vector2& rhs) const;
        Real    DistanceSquaredTo(const Vector2& rhs) const;
        Vector2 ProjectionTo(const Vector2& rhs) const;
        Vector2 ProjectionFrom(const Vector2& rhs) const;
        Vector2 Unit() const;
        Vector2 Half() const;
        Vector2 Inverse() const;

        Real     DotProduct(const Vector2& rhs) const;
        Real     CrossProduct(const Vector2& rhs) const;
        Vector2  CrossProduct(const Real& rhs) const;
        Matrix22 OuterProduct(const Vector2& rhs) const;
        Vector2  HadamardProduct(const Vector2& rhs) const;

        bool IsValid() const;
        bool IsZero() const;
        bool IsEqual(const Vector2& rhs) const;
        bool IsNotEqual(const Vector2& rhs) const;

        Real GrepVec1(size_t flag0) const;
        Vector2 GrepVec2(size_t flag0, size_t flag1) const;
        Vector3 GrepVec3(size_t flag0, size_t flag1, size_t flag2) const;
        Vector4 GrepVec4(size_t flag0, size_t flag1, size_t flag2, size_t flag3) const;
        size_t SafeFlag(size_t given) const;

        friend Real    DotProduct(const Vector2& vec1, const Vector2& vec2);
        friend Real    CrossProduct(const Vector2& vec1, const Vector2& vec2);
        friend Vector2 CrossProduct(Real vec1, const Vector2& vec2);
        friend Vector2 CrossProduct(const Vector2& vec1, Real vec2);

        friend Matrix22 OuterProduct(const Vector2& vec1, const Vector2& vec2);
        friend Vector2  HadamardProduct(const Vector2& vec1, const Vector2& vec2);
        friend Vector2  Projection(const Vector2& vec1, const Vector2& vec2);
        friend Vector2  LinearInterpolation(const Vector2& start, const Vector2& end, Real t);

    public:
        bool                 operator ==(const Vector2& rhs) const;
        bool                 operator !=(const Vector2& rhs) const;
        Vector2              operator -() const;
        Vector2&             operator =(const Vector2& rhs);
        Vector2&             operator =(const Vector3& rhs);
        Vector2&             operator =(const Vector4& rhs);
        Vector2&             operator =(Real rhs);
        Vector2&             operator +=(const Vector2& rhs);
        Vector2&             operator +=(Real real);
        Vector2&             operator -=(const Vector2& rhs);
        Vector2&             operator -=(Real real);
        Vector2&             operator *=(Real real);
        Vector2&             operator /=(Real real);
        Vector2              operator +(const Vector2& rhs) const;
        Vector2              operator +(Real real) const;
        Vector2              operator -(const Vector2& rhs) const;
        Vector2              operator -(Real real) const;
        Vector2              operator *(Real real) const;
        Vector2              operator /(Real real) const;
        friend std::ostream& operator <<(std::ostream& os, const Vector2& rhs);
        friend Vector2       operator *(Real real, const Vector2& vector);
        Vector2&             operator ++();
        Vector2              operator ++(int);
        Vector2&             operator --();
        Vector2              operator --(int);
        Real                 operator[](size_t i) const;
        Real&                operator[](size_t i);

    public:
        Real x, y;
    };
}
