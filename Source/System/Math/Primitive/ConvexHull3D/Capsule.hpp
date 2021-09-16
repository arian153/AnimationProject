#pragma once
#include "../Primitive.hpp"


namespace CS460
{
    class Capsule final : public Primitive
    {
    public:
        Capsule();
        ~Capsule();

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

        Real    HalfHeight() const;
        Vector3 Radius() const;

    private:
        bool TestRaySphere(const Ray& ray, const Vector3& centroid, Real& min_t, Real& max_t) const;

    public:
        Real radius = 0.25f;
        Real height = 0.5f;
    };
}
