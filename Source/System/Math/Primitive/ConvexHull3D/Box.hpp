#pragma once
#include "../Primitive.hpp"


namespace CS460
{
    class Box final : public Primitive
    {
    public:
        Box();
        ~Box();

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

        void SetBox(Real width, Real height, Real depth);
    public:
        Vector3 vertices[8];
    };
}
