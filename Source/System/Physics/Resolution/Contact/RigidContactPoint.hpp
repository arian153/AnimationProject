#pragma once
#include "../../../Math/Math.hpp"

namespace CS460
{
    class ColliderPrimitive;

    class RigidContactPoint
    {
    public:
        RigidContactPoint();
        ~RigidContactPoint();
        void          Clear();
        RigidContactPoint& operator=(const RigidContactPoint& rhs);
        bool          operator==(const RigidContactPoint& rhs) const;
        void          Swap();
        void          UpdateContactPoint(const RigidContactPoint& rhs);
        RigidContactPoint  SwappedContactPoint() const;
    public:
        //Two contact points in world space, each representing the deepest penetrating point of one collider.
        Vector3 global_position_a;
        Vector3 global_position_b;

        //Two contact points in local spaces of individual colliders, each corresponding to a contact point in world space. 
        //This information is crucial for maintaining persistent contacts. 
        Vector3 local_position_a;
        Vector3 local_position_b;

        //Contact normal representing the "best direction" to separate the two colliding colliders.
        Vector3 normal;
        Vector3 r_a;
        Vector3 r_b;

        //Penetration depth, a scalar value that represents how deep the overlap of the two colliders.
        Real depth = 0.0f;

        //lagrangian multiplier sum.
        Real normal_lambda    = 0.0f;
        Real tangent_lambda   = 0.0f;
        Real bitangent_lambda = 0.0f;

        ColliderPrimitive* collider_a = nullptr;
        ColliderPrimitive* collider_b = nullptr;

        bool b_valid      = true;
        bool b_persistent = false;
    };
}
