#include "ColliderSet.hpp"
#include "World.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderBox.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderCapsule.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderCone.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderCylinder.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderDome.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderEllipsoid.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderPolyhedron.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderSphere.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderTetrahedron.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderTruncated.hpp"
#include "../../../Manager/Component/EngineComponent/ColliderComponent.hpp"
#include "../ColliderPrimitive/ConvexHull2D/ColliderTriangle.hpp"
#include "../ColliderPrimitive/ConvexHull2D/ColliderRectangle.hpp"
#include "../ColliderPrimitive/ConvexHull2D/ColliderCircle.hpp"
#include "../ColliderPrimitive/ConvexHull2D/ColliderEllipse.hpp"
#include "../ColliderPrimitive/ConvexHull2D/ColliderPolygon.hpp"

namespace CS460
{
    ColliderSet::ColliderSet(World* world)
        : m_world(world)
    {
    }

    ColliderSet::~ColliderSet()
    {
    }

    void ColliderSet::Initialize()
    {
        if (m_colliders == nullptr)
        {
            m_colliders = new std::vector<ColliderPrimitive*>;
        }
    }

    void ColliderSet::Shutdown()
    {
        if (m_colliders != nullptr)
        {
            for (auto& collider_data : *m_colliders)
            {
                collider_data->Shutdown();
                delete collider_data;
                collider_data = nullptr;
            }
            m_colliders->clear();
            delete m_colliders;
            m_colliders = nullptr;
        }
        if (m_component != nullptr)
        {
            m_component->m_collider_set = nullptr;
        }
    }

    ColliderPrimitive* ColliderSet::AddCollider(eColliderType type)
    {
        ColliderPrimitive* primitive;
        switch (type)
        {
        case CS460::eColliderType::Circle:
            primitive = new ColliderCircle();
            break;
        case CS460::eColliderType::Ellipse:
            primitive = new ColliderEllipse();
            break;
        case CS460::eColliderType::Polygon:
            primitive = new ColliderPolygon();
            break;
        case CS460::eColliderType::Rectangle:
            primitive = new ColliderRectangle();
            break;
        case CS460::eColliderType::Triangle:
            primitive = new ColliderTriangle();
            break;
        case CS460::eColliderType::Box:
            primitive = new ColliderBox();
            break;
        case CS460::eColliderType::Capsule:
            primitive = new ColliderCapsule();
            break;
        case CS460::eColliderType::Cone:
            primitive = new ColliderCone();
            break;
        case CS460::eColliderType::Cylinder:
            primitive = new ColliderCylinder();
            break;
        case CS460::eColliderType::Dome:
            primitive = new ColliderDome();
            break;
        case CS460::eColliderType::Ellipsoid:
            primitive = new ColliderEllipsoid();
            break;
        case CS460::eColliderType::Polyhedron:
            primitive = new ColliderPolyhedron();
            break;
        case CS460::eColliderType::Sphere:
            primitive = new ColliderSphere();
            break;
        case CS460::eColliderType::Tetrahedron:
            primitive = new ColliderTetrahedron();
            break;
        case CS460::eColliderType::Truncated:
            primitive = new ColliderTruncated();
            break;
        case CS460::eColliderType::InvalidPrimitive:
            return nullptr;
        default:
            return nullptr;
        }
        primitive->m_collider_set = this;
        if (m_rigid_body != nullptr)
        {
            primitive->m_rigid_body = m_rigid_body;
        }
        if (m_world != nullptr)
        {
            m_world->AddPrimitive(primitive);
        }
        primitive->Initialize();
        m_colliders->push_back(primitive);
        return primitive;
    }

    ColliderPrimitive* ColliderSet::GetCollider(size_t index) const
    {
        if (m_colliders != nullptr)
        {
            auto size = m_colliders->size();
            if (index < size)
            {
                return m_colliders->at(index);
            }
        }
        return nullptr;
    }

    void ColliderSet::EraseCollider(ColliderPrimitive* collider) const
    {
        auto end = m_colliders->end();
        for (auto it = m_colliders->begin(); it != end;)
        {
            if (*it == collider)
            {
                m_colliders->erase(it++);
            }
            else
            {
                ++it;
            }
        }
    }

    RigidBody* ColliderSet::GetRigidBody() const
    {
        return m_rigid_body;
    }

    void ColliderSet::SetMass(Real density)
    {
        if (m_colliders != nullptr)
        {
            // compute local centroid & mass
            for (auto& collider_data : *m_colliders)
            {
                collider_data->SetMassData(density);
            }
            UpdateMassData();
        }
    }

    void ColliderSet::SetScale(const Vector3& scale)
    {
        if (m_colliders != nullptr)
        {
            for (auto& collider_data : *m_colliders)
            {
                collider_data->SetScaleData(scale);
                if (collider_data->m_bounding_volume != nullptr)
                {
                    collider_data->UpdateBoundingVolume();
                }
                collider_data->SetMassData(collider_data->m_material.density);
            }
            UpdateMassData();
            UpdateBoundingVolume();
        }
    }

    void ColliderSet::SetRigidBody(RigidBody* rigid_body)
    {
        m_rigid_body = rigid_body;
        for (auto& collider : *m_colliders)
        {
            collider->m_rigid_body = rigid_body;
            collider->UpdateRigidBody();
        }
        UpdateBoundingVolume();
    }

    MassData ColliderSet::GetMassData() const
    {
        return m_mass_data;
    }

    Vector3 ColliderSet::GetScale() const
    {
        return m_scale;
    }

    void ColliderSet::UpdateMassData()
    {
        m_mass_data.mass = 0.0f;
        m_mass_data.local_centroid.SetZero();
        if (m_colliders != nullptr)
        {
            // compute local centroid & mass
            for (auto& collider_data : *m_colliders)
            {
                // accumulate mass
                m_mass_data.mass += collider_data->m_mass;
                // accumulate weighted contribution
                m_mass_data.local_centroid += collider_data->m_mass * collider_data->WorldCentroid();
            }
            // compute inverse mass
            if (Math::IsZero(m_mass_data.mass) == false)
            {
                m_mass_data.inverse_mass = 1.0f / m_mass_data.mass;
            }
            else
            {
                //infinite mass.
                m_mass_data.inverse_mass = 0.0f;
            }
            // compute final local centroid
            m_mass_data.local_centroid *= m_mass_data.inverse_mass;
            // compute local inertia tensor
            m_colliders->empty()
                ? m_mass_data.local_inertia.SetIdentity()
                : m_mass_data.local_inertia.SetZero();
            for (auto& collider_data : *m_colliders)
            {
                Vector3  r       = m_mass_data.local_centroid - collider_data->WorldCentroid();
                Real     r_dot_r = r.DotProduct(r);
                Matrix33 r_out_r = r.OuterProduct(r);
                // accumulate local inertia tensor contribution, using Parallel Axis Theorem
                m_mass_data.local_inertia += collider_data->WorldInertia() + collider_data->m_mass * (r_dot_r * Matrix33::Identity() - r_out_r);
            }
            // compute inverse inertia tensor
            m_mass_data.local_inverse_inertia = m_mass_data.local_inertia.Inverse();
        }
        if (m_rigid_body != nullptr)
        {
            m_rigid_body->SetMassData(m_mass_data);
        }
    }

    void ColliderSet::SyncToTransform(Transform* transform) const
    {
        transform->scale = m_scale;
    }

    void ColliderSet::SyncFromTransform(Transform* transform)
    {
        SetScale(transform->scale);
    }

    void ColliderSet::UpdateBoundingVolume()
    {
        if (m_colliders != nullptr)
        {
            if (!m_colliders->empty())
            {
                auto collider_data = m_colliders->at(0);
                collider_data->UpdateBoundingVolume();
                m_bounding_volume.Set(collider_data->m_bounding_volume->Min(), collider_data->m_bounding_volume->Max());

                for (size_t i = 1; i < m_colliders->size(); ++i)
                {
                    collider_data = m_colliders->at(i);
                    collider_data->UpdateBoundingVolume();
                    m_bounding_volume = collider_data->m_bounding_volume->Union(m_bounding_volume);
                }

                m_bounding_volume.ExpandMargin(Physics::Collision::BROAD_PHASE_MARGIN * 2);
            }
        }
    }

    void ColliderSet::UpdateUnitScale()
    {
        /* if (m_colliders != nullptr)
         {
             size_t size = m_colliders->size();
             if (size > 0)
             {
                 Vector3Pair result_min_max = (*m_colliders)[0]->GetMinMax();
 
                 for (size_t i = 1; i < size; ++i)
                 {
                     Vector3Pair min_max = (*m_colliders)[i]->GetMinMax();
                     if (min_max.a.x < result_min_max.a.x)
                         result_min_max.a.x = min_max.a.x;
                 }
             }
         }*/
    }

    void ColliderSet::Clone(ColliderSet* origin, RigidBody* body, World* world)
    {
        if (origin != this)
        {
            m_rigid_body = body;
            m_world      = world;
            m_mass_data  = origin->m_mass_data;
            m_scale      = origin->m_scale;
            for (auto& collider : *origin->m_colliders)
            {
                auto created = this->AddCollider(collider->m_type);
                created->Clone(collider);
            }

            m_rigid_body = body;
            m_world      = world;
            m_mass_data  = origin->m_mass_data;
            m_scale      = origin->m_scale;
            if (m_rigid_body != nullptr)
            {
                m_rigid_body->SetMassData(m_mass_data);
            }
        }
    }

    ColliderComponent* ColliderSet::GetComponent() const
    {
        return m_component;
    }
}
