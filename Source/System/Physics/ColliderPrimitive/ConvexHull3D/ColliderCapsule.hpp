#pragma once
#include "../ColliderPrimitive.hpp"

namespace CS460
{
    /**
     * \brief 
     * Capsule Collider is aligned on the y-axis
     */
    class ColliderCapsule final : public ColliderPrimitive
    {
    public:
        ColliderCapsule();
        ~ColliderCapsule() override;
        ColliderCapsule(const ColliderCapsule& rhs)            = delete;
        ColliderCapsule& operator=(const ColliderCapsule& rhs) = delete;

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

        Real    HalfHeight() const;
        Real    Height() const;
        Vector3 Radius() const;
        void SetCapsule(Real height, const Vector3& radius);
        void SetHeight(Real height);
        void SetRadius(const Vector3& radius);

    protected:
        void Clone(ColliderPrimitive* origin) override;
        void Load(const Json::Value& data) override;
        void Save(const Json::Value& data) override;
        void EditPrimitive(CommandRegistry* registry) override;

    private:
        bool TestRayEllipsoid(const Ray& ray, const Vector3& centroid, Real& min_t, Real& max_t) const;

    private:
        Vector3 m_radius;
        Vector3 m_scaled_radius;
        Real    m_height             = 1.0f;
        Real    m_scaled_height = 1.0f;
    };
}
