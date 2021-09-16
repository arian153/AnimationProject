#pragma once
#include "../ColliderPrimitive.hpp"

namespace CS460
{
    class ColliderEllipse final : public ColliderPrimitive
    {
    public:
        ColliderEllipse();
        ~ColliderEllipse() override;
        ColliderEllipse(const ColliderEllipse& rhs)            = delete;
        ColliderEllipse& operator=(const ColliderEllipse& rhs) = delete;

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

        Vector2 Radius() const;
        void SetEllipse(const Vector2& radius);

    protected:
        void Clone(ColliderPrimitive* origin) override;
        void Load(const Json::Value& data) override;
        void Save(const Json::Value& data) override;
        void EditPrimitive(CommandRegistry* registry) override;

    private:
        Vector2 m_radius;
        Vector2 m_scaled_radius;
    };
}
