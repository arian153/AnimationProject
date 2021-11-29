#include "StepData.hpp"

namespace CS460
{
    StepData::StepData()
    {
    }

    StepData::~StepData()
    {
    }

    StepData::StepData(const Vector3& lvk, const Vector3& lxk, const Vector3& rvk, const Quaternion& rxk)
        : linear_velocity_step(lvk), linear_position_step(lxk), angular_velocity_step(rvk), angular_rotation_step(rxk)
    {
    }
}
