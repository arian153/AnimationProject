#include "ColliderPlane.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"

namespace CS460
{
    ColliderPlane::ColliderPlane()
    {
    }

    ColliderPlane::~ColliderPlane()
    {
    }

    void ColliderPlane::Initialize()
    {
    }

    void ColliderPlane::Shutdown()
    {
    }

    Vector3 ColliderPlane::Support(const Vector3& direction)
    {
        return direction;
    }

    bool ColliderPlane::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        return false;
    }

    Vector3 ColliderPlane::GetNormal(const Vector3& local_point_on_collider) const
    {
        return m_normal;
    }

    void ColliderPlane::SetMassData(Real density)
    {
    }

    Real ColliderPlane::GetVolume()
    {
        return 0.0f;
    }


    void ColliderPlane::UpdateBoundingVolume() const
    {
        m_bounding_volume->Set(Math::Vector3::ORIGIN, Math::Vector3::ORIGIN);
    }

    void ColliderPlane::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
    }
}
