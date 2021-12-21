#pragma once
#include "../../../Math/Math.hpp"
#include "RigidContactPoint.hpp"
#include "../../Utility/PhysicsDef.hpp"

namespace CS460
{
    class RigidContactPoint;
    class RigidBody;
    class ColliderSet;

    class RigidContactManifold
    {
    public:
        RigidContactManifold() = delete;
        explicit RigidContactManifold(ColliderSet* a, ColliderSet* b);
        ~RigidContactManifold();
        RigidContactManifold(const RigidContactManifold& rhs);
        RigidContactManifold& operator=(const RigidContactManifold& rhs);

        void   Set(const RigidContactManifold& manifold);
        void   UpdateInvalidContact();
        void   UpdateCurrentManifold(const RigidContactPoint& new_contact);
        void   CutDownManifold();
        size_t ContactsCount() const;
        void   ClearContacts();

    private:
        Real DistanceFromPoint(const RigidContactPoint& contact, RigidContactPoint* p0);
        Real DistanceFromLineSegment(const RigidContactPoint& contact, RigidContactPoint* p0, RigidContactPoint* p1);
        Real DistanceFromTriangle(const RigidContactPoint& contact, RigidContactPoint* p0, RigidContactPoint* p1, RigidContactPoint* p2);
        bool OnTriangle(RigidContactPoint* point, RigidContactPoint* p0, RigidContactPoint* p1, RigidContactPoint* p2);

    private:
        friend class Resolution;
        friend class NarrowPhase;
        friend class ContactConstraint;
        friend class ManifoldTable;

    private:
        bool is_collide = false;

        //data
        ColliderSet* m_set_a = nullptr;
        ColliderSet* m_set_b = nullptr;

        std::vector<RigidContactPoint> contacts;
    };
}
