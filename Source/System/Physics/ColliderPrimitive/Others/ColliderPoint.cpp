#include "ColliderPoint.hpp"

namespace CS460
{
    ColliderPoint::ColliderPoint()
    {
    }

    ColliderPoint::~ColliderPoint()
    {
    }

    void ColliderPoint::Initialize()
    {
    }

    void ColliderPoint::Shutdown()
    {
    }

    Vector3 ColliderPoint::Support(const Vector3& direction)
    {
        return Vector3();
    }

    bool ColliderPoint::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real a = local_ray.direction.x * local_ray.direction.x
                + local_ray.direction.y * local_ray.direction.y
                + local_ray.direction.z * local_ray.direction.z;
        Real b = 2.0f * (local_ray.direction.x * local_ray.position.x
            + local_ray.direction.y * local_ray.position.y
            + local_ray.direction.z * local_ray.position.z);
        Real c = local_ray.position.x * local_ray.position.x
                + local_ray.position.y * local_ray.position.y
                + local_ray.position.z * local_ray.position.z;

        if (Math::SolveQuadratic(a, b, c, maximum_t, minimum_t) == true)
        {
            if (minimum_t < 0.0f && maximum_t < 0.0f)
            {
                return false;
            }
            if (minimum_t <= 0.0f)
            {
                minimum_t = 0.0f;
            }
            return true;
        }
        return false;
    }

    Vector3 ColliderPoint::GetNormal(const Vector3& local_point_on_collider)
    {
        //there is no normal...
        return local_point_on_collider.Normalize();
    }

    void ColliderPoint::SetMassData(Real density)
    {
    }

    Real ColliderPoint::GetVolume()
    {
        return 0.0f;
    }

    void ColliderPoint::SetScaleData(const Vector3& scale)
    {
    }

    void ColliderPoint::SetUnit()
    {
    }

    void ColliderPoint::UpdateBoundingVolume()
    {
    }

    void ColliderPoint::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
    }

    void ColliderPoint::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderPoint* sphere = static_cast<ColliderPoint*>(origin);
            //collider local space data
            m_local = sphere->m_local;
            //collider mass data
            m_centroid             = sphere->m_centroid;
            m_mass                 = sphere->m_mass;
            m_local_inertia_tensor = sphere->m_local_inertia_tensor;
            m_material             = sphere->m_material;
        }
    }

    void ColliderPoint::Load(const Json::Value& data)
    {
        LoadTransform(data);
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderPoint::Save(const Json::Value& data)
    {
    }

    void ColliderPoint::EditPrimitive(CommandRegistry* registry)
    {
    }
}
