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
        Real    mass             = 1.0f;
        Real    inverse_mass     = 1.0f;
        Vector3 local_position;

        //global value
        Vector3 world_position;
        Vector3 linear_velocity;
        Vector3 force_accumulator;

        int idx = -1;
    };

    class SoftBodySpring
    {
    public:
        SoftBodySpring()  = default;
        ~SoftBodySpring() = default;

    public:
        int id_a = -1; //anchor a id
        int id_b = -1; //anchor b id

        Real K = 1.0f;
        Real D = 1.0f;
    };
}
