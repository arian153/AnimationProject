#pragma once
#include "P2Transform.h"
#include "../Math2D/P2Math.h"

namespace P2D
{
    class Collider;

    class RigidBody
    {
    public:
        RigidBody();
        ~RigidBody();

    public:
        Collider* collider = nullptr;
        Transform transform;

        //mass data
        Real    mass        = 1;
        Real    inv_mass    = 1;
        Real    inertia     = 1;
        Real    inv_inertia = 1;
        Vector2 local_centroid;
        Vector2 world_centroid;
    };
}
