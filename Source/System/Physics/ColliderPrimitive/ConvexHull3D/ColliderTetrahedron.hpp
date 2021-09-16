#pragma once
#include "../ColliderPrimitive.hpp"

namespace CS460
{
    class ColliderFace;

    class ColliderTetrahedron final : public ColliderPrimitive
    {
    public:
        ColliderTetrahedron();
        ~ColliderTetrahedron() override;
        ColliderTetrahedron(const ColliderTetrahedron& rhs)            = delete;
        ColliderTetrahedron& operator=(const ColliderTetrahedron& rhs) = delete;

        void Initialize() override;
        void Shutdown() override;

        //minkowski support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //ray - collider intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_collider) override;

        //physics
        void SetMassData(Real density) override;
        Real GetVolume() override;

        //collider 
        void SetScaleData(const Vector3& scale) override;
        void SetUnit() override;
        void UpdateBoundingVolume() override;
        void Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const override;

        Vector3 Vertex(size_t i) const;
        void    SetTetrahedron(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3);

    protected:
        void Clone(ColliderPrimitive* origin) override;
        void Load(const Json::Value& data) override;
        void Save(const Json::Value& data) override;
        void EditPrimitive(CommandRegistry* registry) override;

    private:
        bool IntersectRayFace(const Ray& ray, const ColliderFace& face, Real& t) const;
        bool IsFaceContainPoint(const ColliderFace& face, const Vector3& point, Vector3& normal) const;

    private:
        Vector3 m_vertices[4];
        Vector3 m_scaled_vertices[4];
    };
}
