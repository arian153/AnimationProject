#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../BroadPhase/BoundingAABB.hpp"
#include "MassData.hpp"

namespace CS460
{
    class Simplex;
    class RigidBody;
    enum class eColliderType;
    class ColliderPrimitive;
    class World;
    class NarrowPhase;

    class ColliderSet
    {
    private:
        friend class World;
        friend class RigidBody;
        friend class ColliderComponent;
        friend class ManifoldTable;
        friend class RigidContactManifold;
    public:
        explicit ColliderSet(World* world);
        ~ColliderSet();

        void Initialize();
        void Shutdown();

        //primitives
        ColliderPrimitive* AddCollider(eColliderType type);
        ColliderPrimitive* GetCollider(size_t index) const;
        void               EraseCollider(ColliderPrimitive* collider) const;

        RigidBody* GetRigidBody() const;

        //setters
        void SetMass(Real density);
        void SetScale(const Vector3& scale);
        void SetRigidBody(RigidBody* rigid_body);
        void SetGhost(bool is_ghost);

        bool IsGhost() const;

        //getters
        MassData GetMassData() const;
        Vector3  GetScale() const;

        //modify data
        void UpdateMassData();
        void SyncToTransform(Transform* transform) const;
        void SyncFromTransform(Transform* transform);
        void UpdateBoundingVolume();
        void UpdateUnitScale();

        void Clone(ColliderSet* origin, RigidBody* body, World* world);

        ColliderComponent* GetComponent() const;


        bool GJKDistanceTest(ColliderPrimitive* collider, const Vector3& point, Simplex& simplex) const;

    private:
        RigidBody*         m_rigid_body = nullptr;
        World*             m_world      = nullptr;
        MassData           m_mass_data;
        Vector3            m_scale = Vector3(1.0f, 1.0f, 1.0f);
        BoundingAABB       m_bounding_volume;
        ColliderComponent* m_component = nullptr;
        bool               m_is_ghost  = false;

        //primitives
        std::vector<ColliderPrimitive*>* m_colliders = nullptr;
    };
}
