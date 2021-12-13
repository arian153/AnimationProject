#pragma once
#include "../../Math/Structure/Transform.hpp"
#include "../Utility/PhysicsDef.hpp"

namespace CS460
{
    class Transform;

    class SoftBody
    {
    public:
        SoftBody();
        ~SoftBody();

    private:



        eMotionMode m_motion_mode = eMotionMode::Dynamic;
        Transform*  m_transform   = nullptr;
        Transform   m_local_copy;

        bool m_b_sleep = false;
        Real m_sleep_momentum = Physics::Collision::SLEEP_AWAKE;
    };
}
