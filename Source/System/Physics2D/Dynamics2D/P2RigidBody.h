#pragma once
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
        Vector2   position;
        Real      rotation = 0;

        //mass data
        Real    mass        = 1;
        Real    inv_mass    = 1;
        Real    inertia     = 1;
        Real    inv_inertia = 1;
        Vector2 local_centroid;
        Vector2 world_centroid;
    };
}
