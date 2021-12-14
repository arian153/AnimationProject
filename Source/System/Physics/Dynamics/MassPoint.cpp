#include "MassPoint.hpp"

#include "../../Math/Utility/Utility.inl"

namespace CS460
{
    void MassPoint::SetMass(Real mass)
    {
        inverse_mass = Math::IsZero(mass) ? 0.0f : 1.0f / mass;
    }

    Vector3 MassPoint::Support(const Vector3& direction) const
    {
        return effective_radius * direction;
    }
}
