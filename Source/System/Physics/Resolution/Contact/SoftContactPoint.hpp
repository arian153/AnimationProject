#pragma once
#include "../../../Math/Algebra/Vector3.hpp"

namespace CS460
{
    class MassPoint;
    class SoftBody;
    class ColliderPrimitive;

    class SoftContactPoint
    {
    public:
        SoftContactPoint();
        ~SoftContactPoint();

        void              Clear();
        SoftContactPoint& operator=(const SoftContactPoint& rhs);
        bool              operator==(const SoftContactPoint& rhs) const;
        void              Swap();
        void              UpdateContactPoint(const SoftContactPoint& rhs);
        SoftContactPoint  SwappedContactPoint() const;
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

        ColliderPrimitive* collider_a = nullptr; // rigid vs soft
        SoftBody*          softbody_a = nullptr; //  soft vs soft
        SoftBody*          softbody_b = nullptr; 

        int  mass_point_idx_a = -1;
        int  mass_point_idx_b = -1;
        bool b_soft_vs_soft   = false;

        bool b_valid      = true;
        bool b_persistent = false;
    };
}
