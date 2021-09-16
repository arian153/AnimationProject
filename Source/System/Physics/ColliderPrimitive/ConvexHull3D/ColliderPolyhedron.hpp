#pragma once
#include "../ColliderPrimitive.hpp"
#include "../ColliderEdge.hpp"
#include "../ColliderFace.hpp"
#include "../OutsideSetFace.hpp"

namespace CS460
{
    class ColliderPolyhedron final : public ColliderPrimitive
    {
    public:
        ColliderPolyhedron();
        ~ColliderPolyhedron() override;
        ColliderPolyhedron(const ColliderPolyhedron& rhs)            = delete;
        ColliderPolyhedron& operator=(const ColliderPolyhedron& rhs) = delete;

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
        size_t  Size() const;
        bool SetPolyhedron(const std::vector<Vector3>& vertices);

    protected:
        void Clone(ColliderPrimitive* origin) override;
        void Load(const Json::Value& data) override;
        void Save(const Json::Value& data) override;
        void EditPrimitive(CommandRegistry* registry) override;

    private:
        class SubMassData
        {
        public:
            Real     mass = 0.0f;
            Vector3  centroid;
            Matrix33 inertia;
        };

    private:
        bool        IntersectRayFace(const Ray& ray, const ColliderFace& face, Real& t) const;
        bool        IsFaceContainPoint(const ColliderFace& face, const Vector3& point, Vector3& normal) const;
        Matrix33    TranslateInertia(const Matrix33& input, const Vector3& centroid, Real mass, const Vector3& offset) const;
        SubMassData CalculateTetrahedronMassData(const Vector3& ref, const Vector3& v1, const Vector3& v2, const Vector3& v3, Real density) const;
        void CalculateMinMaxBound();

        size_t CreateSimplex(const std::vector<Vector3>& vertices) const;
        void AddToOutsideSet(std::vector<Vector3>& vertices, OutsideSetFace& result) const;
        void CalculateHorizon();

    private:
        std::vector<Vector3>*      m_vertices        = nullptr;
        std::vector<Vector3>*      m_scaled_vertices = nullptr;
        std::vector<ColliderEdge>* m_edges           = nullptr;
        std::vector<ColliderFace>* m_faces           = nullptr;

        Vector3 m_min_bound;
        Vector3 m_max_bound;
    };
}
