#pragma once
#include "../Primitive.hpp"
#include "../../Algebra/Vector2.hpp"

namespace CS460
{
    class Triangle final : public Primitive
    {
    public:
        Triangle();
        explicit Triangle(const Vector2& p0, const Vector2& p1, const Vector2& p2);
        explicit Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);
        ~Triangle();

        void Initialize() override;
        void Shutdown() override;

        //Primitive Data
        void SetUnit() override;

        //Minkowski Support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //Ray - Primitive Intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_primitive) override;

        //Draw
        void DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const override;

        void SetTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2);
        void SetTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);

        Vector3 Vertex(size_t i) const;
        Vector3 ClosestPoint(const Vector3& point) const;

        Real Distance(const Vector3& point) const;
        Real DistanceSquared(const Vector3& point) const;

    public:
        static Vector3 ClosestPoint(const Vector3& point, const Vector3& p0, const Vector3& p1, const Vector3& p2);
        static Real DistanceSquared(const Vector3& point, const Vector3& p0, const Vector3& p1, const Vector3& p2);
        static Real Distance(const Vector3& point, const Vector3& p0, const Vector3& p1, const Vector3& p2);
        static bool IsContainPoint(const Vector3& point, const Vector3& p0, const Vector3& p1, const Vector3& p2);
        static Vector3 Normal(const Vector3& p0, const Vector3& p1, const Vector3& p2);

    public:
        Vector2 vertices[3];
    };
}
