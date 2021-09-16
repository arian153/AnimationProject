#pragma once
#include "../ColliderPrimitive.hpp"
#include "../ColliderEdge.hpp"

namespace CS460
{
    class ColliderPolygon final : public ColliderPrimitive
    {
    public:
        ColliderPolygon();
        ~ColliderPolygon() override;
        ColliderPolygon(const ColliderPolygon& rhs)            = delete;
        ColliderPolygon& operator=(const ColliderPolygon& rhs) = delete;

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

        Vector2 Vertex(size_t i) const;

        bool SetPolygon(const std::vector<Vector2>& vertices);


    protected:
        void Clone(ColliderPrimitive* origin) override;
        void Load(const Json::Value& data) override;
        void Save(const Json::Value& data) override;
        void EditPrimitive(CommandRegistry* registry) override;

    private:
        void CreateSimplex();
        void AddToOutsideSet();
        void CalculateHorizon();

    private:
        std::vector<Vector2>*      m_vertices        = nullptr;
        std::vector<Vector2>*      m_scaled_vertices = nullptr;
        std::vector<ColliderEdge>* m_edges           = nullptr;

        Vector2 m_min_bound;
        Vector2 m_max_bound;
    };
}
