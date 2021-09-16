#pragma once
#include "../Primitive.hpp"
#include <vector>

namespace CS460
{
    class Polyhedron final : public Primitive
    {
    public:
        Polyhedron();
        ~Polyhedron();

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

        void UpdateMinMaxPoint();

    public:
        std::vector<Vector3>* vertices = nullptr;

    private:
        //max x, max y, max z among vertices
        Vector3 max_point;
        //min x, min y, min z among vertices
        Vector3 min_point;
    };
}
