#include "MassPoint.hpp"

#include "../../Math/Utility/Utility.inl"

namespace CS460
{
    void MassPoint::SetMass(Real _mass)
    {
        mass = _mass;
        inverse_mass = Math::IsZero(_mass) ? 0.0f : 1.0f / _mass;
    }

    Vector3 MassPoint::Support(const Vector3& direction) const
    {
        return effective_radius * direction;
    }
}
