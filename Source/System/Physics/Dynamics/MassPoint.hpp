#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class MassPoint
    {
    public:
        MassPoint()  = default;
        ~MassPoint() = default;
        void SetMass(Real mass);

        Vector3 Support(const Vector3& direction) const;

    public:
        //local value
        Real    effective_radius = 0.1f;
        Real    inverse_mass     = 1.0f;
        Vector3 local_position;

        //global value
        Vector3 world_position;
        Vector3 linear_velocity;
        Vector3 force_accumulator;
    };
}
