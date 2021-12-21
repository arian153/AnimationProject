#include "RigidContactManifold.hpp"
#include <list>
#include "../../ColliderPrimitive/ColliderPrimitive.hpp"
#include "../../Dynamics/RigidBody.hpp"
#include "../../Dynamics/ColliderSet.hpp"

namespace CS460
{
    RigidContactManifold::RigidContactManifold(ColliderSet* a, ColliderSet* b)
        : m_set_a(a), m_set_b(b)
    {
    }

    RigidContactManifold::~RigidContactManifold()
    {
    }

    RigidContactManifold::RigidContactManifold(const RigidContactManifold& rhs)
    {
        m_set_a    = rhs.m_set_a;
        m_set_b    = rhs.m_set_b;
        is_collide = rhs.is_collide;
        for (auto& contact : rhs.contacts)
        {
            contacts.push_back(contact);
        }
    }

    RigidContactManifold& RigidContactManifold::operator=(const RigidContactManifold& rhs)
    {
        if (this != &rhs)
        {
            m_set_a    = rhs.m_set_a;
            m_set_b    = rhs.m_set_b;
            is_collide = rhs.is_collide;
            for (auto& contact : rhs.contacts)
            {
                contacts.push_back(contact);
            }
        }
        return *this;
    }

    void RigidContactManifold::Set(const RigidContactManifold& manifold)
    {
        m_set_a    = manifold.m_set_a;
        m_set_b    = manifold.m_set_b;
        is_collide = manifold.is_collide;
        for (auto& contact : manifold.contacts)
        {
            contacts.push_back(contact);
        }
    }

    void RigidContactManifold::UpdateInvalidContact()
    {
        //erase contact list.
        std::list<RigidContactPoint*> remove_list;
        for (auto& contact : contacts)
        {
            //convert existing contact point from local space to world space.
            //if both bodies are far enough away, remove contact from manifold data.
            Vector3 local_to_global_a = m_set_a->m_rigid_body->LocalToWorldPoint(contact.collider_a->LocalToWorldPoint(contact.local_position_a));
            Vector3 local_to_global_b = m_set_b->m_rigid_body->LocalToWorldPoint(contact.collider_b->LocalToWorldPoint(contact.local_position_b));
            //current frame's distance between a to b.
            Vector3 r_ab = local_to_global_b - local_to_global_a;
            Vector3 r_a  = contact.global_position_a - local_to_global_a;
            Vector3 r_b  = contact.global_position_b - local_to_global_b;

            //check still penetrate between both bodies.
            bool b_still_penetrating = contact.normal.DotProduct(r_ab) <= 0.0f;
            bool b_r_a_close_enough  = r_a.LengthSquared() < Physics::Collision::PERSISTENT_THRESHOLD_SQUARED;
            bool b_r_b_close_enough  = r_b.LengthSquared() < Physics::Collision::PERSISTENT_THRESHOLD_SQUARED;
            // keep contact point if the collision pair is still colliding at this point, 
            // and the local positions are not too far from the global positions original acquired from collision detection
            if (b_r_a_close_enough && b_r_b_close_enough && b_still_penetrating)
            {
                // contact persistent, keep
                contact.b_persistent = true;
            }
            else
            {
                // contact invalid, remove
                remove_list.push_back(&contact);
            }
        }
        for (auto it = remove_list.begin(); it != remove_list.end(); ++it)
        {
            for (auto itc = contacts.begin(); itc != contacts.end();)
            {
                if (*itc == **it)
                {
                    contacts.erase(itc++);
                    break;
                }
                ++itc;
            }
        }
        remove_list.clear();
    }

    void RigidContactManifold::UpdateCurrentManifold(const RigidContactPoint& new_contact)
    {
        bool discard_contact = false;

        size_t size          = contacts.size();
        size_t replace_index = size;
        for (size_t i = 0; i < size; ++i)
        {
            Vector3 r_a              = new_contact.global_position_a - contacts[i].global_position_a;
            bool    r_a_close_enough = r_a.LengthSquared() < Physics::Collision::PERSISTENT_THRESHOLD_SQUARED;
            // proximity check  
            if (r_a_close_enough)
            {
                //replace new_contact
                discard_contact = true;
                replace_index   = i;
                break;
            }
        }

        if (discard_contact)
        {
            //replace new contact
            contacts[replace_index].UpdateContactPoint(new_contact);
        }
        else
        {
            //add new contact
            auto set_a = new_contact.collider_a->GetColliderSet();
            auto set_b = new_contact.collider_b->GetColliderSet();
            if (set_a == m_set_b && set_b == m_set_a)
            {
                contacts.push_back(new_contact.SwappedContactPoint());
            }
            else
            {
                contacts.push_back(new_contact);
            }
        }
    }

    void RigidContactManifold::CutDownManifold()
    {
        if (contacts.size() <= Physics::Collision::MAX_MANIFOLD_POINT_COUNT)
        {
            return;
        }
        // find the deepest penetrating one
        RigidContactPoint* deepest     = nullptr;
        Real          penetration = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            if (contact.depth > penetration)
            {
                penetration = contact.depth;
                deepest     = &contact;
            }
        }
        // find second contact
        RigidContactPoint* furthest1         = nullptr;
        Real          distance_squared1 = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            Real dist = DistanceFromPoint(contact, deepest);
            if (dist > distance_squared1)
            {
                distance_squared1 = dist;
                furthest1         = &contact;
            }
        }
        // find third contact
        RigidContactPoint* furthest2         = nullptr;
        float         distance_squared2 = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            Real dist = DistanceFromLineSegment(contact, deepest, furthest1);
            if (dist > distance_squared2)
            {
                distance_squared2 = dist;
                furthest2         = &contact;
            }
        }
        // find fourth contact
        RigidContactPoint* furthest3         = nullptr;
        float         distance_squared3 = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            Real dist = DistanceFromTriangle(contact, deepest, furthest1, furthest2);
            if (dist > distance_squared3)
            {
                distance_squared3 = dist;
                furthest3         = &contact;
            }
        }
        contacts.clear();
        if (deepest != nullptr)
        {
            contacts.push_back(*deepest);
        }
        if (furthest1 != nullptr)
        {
            contacts.push_back(*furthest1);
        }
        if (furthest2 != nullptr)
        {
            contacts.push_back(*furthest2);
        }
        bool on_triangle = OnTriangle(furthest3, deepest, furthest1, furthest2);
        if (on_triangle == false && furthest3 != nullptr)
        {
            contacts.push_back(*furthest3);
        }
    }

    size_t RigidContactManifold::ContactsCount() const
    {
        return contacts.size();
    }

    void RigidContactManifold::ClearContacts()
    {
        contacts.clear();
        is_collide = false;
    }

    Real RigidContactManifold::DistanceFromPoint(const RigidContactPoint& contact, RigidContactPoint* p0)
    {
        if (p0 == nullptr)
        {
            return Math::REAL_NEGATIVE_MAX;
        }
        return (contact.global_position_a - p0->global_position_a).LengthSquared();
    }

    Real RigidContactManifold::DistanceFromLineSegment(const RigidContactPoint& contact, RigidContactPoint* p0, RigidContactPoint* p1)
    {
        if (p0 == nullptr || p1 == nullptr)
        {
            return Math::REAL_NEGATIVE_MAX;
        }
        Vector3 n  = p1->global_position_a - p0->global_position_a;
        Vector3 pa = p0->global_position_a - contact.global_position_a;
        Vector3 c  = n * (pa.DotProduct(n) / n.DotProduct(n));
        Vector3 d  = pa - c;
        return d.DotProduct(d);
    }

    Real RigidContactManifold::DistanceFromTriangle(const RigidContactPoint& contact, RigidContactPoint* p0, RigidContactPoint* p1, RigidContactPoint* p2)
    {
        if (p0 == nullptr || p1 == nullptr || p2 == nullptr)
        {
            return Math::REAL_NEGATIVE_MAX;
        }
        Vector3 v0      = p1->global_position_a - p0->global_position_a;
        Vector3 v1      = p2->global_position_a - p0->global_position_a;
        Vector3 v2      = contact.global_position_a - p0->global_position_a;
        Real    d00     = v0.DotProduct(v0);
        Real    d01     = v0.DotProduct(v1);
        Real    d11     = v1.DotProduct(v1);
        Real    d20     = v2.DotProduct(v0);
        Real    d21     = v2.DotProduct(v1);
        Real    denom   = d00 * d11 - d01 * d01;
        Real    v       = (d11 * d20 - d01 * d21) / denom;
        Real    w       = (d00 * d21 - d01 * d20) / denom;
        Real    u       = 1.0f - v - w;
        Vector3 closest = p0->global_position_a * u + p1->global_position_a * v + p2->global_position_a * w;
        return (closest - contact.global_position_a).LengthSquared();
    }

    bool RigidContactManifold::OnTriangle(RigidContactPoint* point, RigidContactPoint* p0, RigidContactPoint* p1, RigidContactPoint* p2)
    {
        if (point == nullptr || p0 == nullptr || p1 == nullptr || p2 == nullptr)
        {
            return false;
        }
        Vector3 edge1  = p1->global_position_a - p0->global_position_a;
        Vector3 edge2  = p2->global_position_a - p1->global_position_a;
        Vector3 normal = edge1.CrossProduct(edge2);
        Vector3 w_test = edge1.CrossProduct(point->global_position_a - p0->global_position_a);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        w_test = edge2.CrossProduct(point->global_position_a - p1->global_position_a);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        Vector3 edge3 = p0->global_position_a - p2->global_position_a;
        w_test        = edge3.CrossProduct(point->global_position_a - p2->global_position_a);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        return true;
    }
}
