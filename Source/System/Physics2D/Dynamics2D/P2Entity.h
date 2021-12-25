#pragma once

namespace P2D
{
    class Collider;
    class RigidBody;

    class Entity
    {
    public:
        Entity();
        ~Entity();

    private:
        RigidBody* m_p2_rigid_body = nullptr;
        Collider*  m_p2_collider   = nullptr;
    };
}
