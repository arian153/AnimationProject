#pragma once
#include "../../Math/Math.hpp"
#include "ContactPoint.hpp"
#include "../Utility/PhysicsDef.hpp"

namespace CS460
{
    class ContactPoint;
    class RigidBody;
    class ColliderSet;

    class ContactManifold
    {
    public:
        ContactManifold() = delete;
        explicit ContactManifold(ColliderSet* a, ColliderSet* b);
        ~ContactManifold();
        ContactManifold(const ContactManifold& rhs);
        ContactManifold& operator=(const ContactManifold& rhs);

        void   Set(const ContactManifold& manifold);
        void   UpdateInvalidContact();
        void   UpdateCurrentManifold(const ContactPoint& new_contact);
        void   CutDownManifold();
        size_t ContactsCount() const;
        void   ClearContacts();

    private:
        Real DistanceFromPoint(const ContactPoint& contact, ContactPoint* p0);
        Real DistanceFromLineSegment(const ContactPoint& contact, ContactPoint* p0, ContactPoint* p1);
        Real DistanceFromTriangle(const ContactPoint& contact, ContactPoint* p0, ContactPoint* p1, ContactPoint* p2);
        bool OnTriangle(ContactPoint* point, ContactPoint* p0, ContactPoint* p1, ContactPoint* p2);

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

        std::vector<ContactPoint> contacts;
    };
}
