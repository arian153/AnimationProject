#pragma once
#include "../../Math/Algebra/Quaternion.hpp"
#include "../../Math/Algebra/Vector3.hpp"

namespace CS460
{
    class StepData
    {
    public:
        StepData();
        ~StepData();

        StepData(const Vector3& lvk, const Vector3& lxk, const Vector3& rvk, const Quaternion& rxk);

    public:
        Vector3    linear_velocity_step;
        Vector3    linear_position_step;
        Vector3    angular_velocity_step;
        Quaternion angular_rotation_step;

        Vector3 force_result;
        Vector3 torque_result;
    };
}
