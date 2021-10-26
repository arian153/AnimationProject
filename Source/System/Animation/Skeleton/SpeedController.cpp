#include "SpeedController.hpp"

#include "../../Math/Utility/Utility.hpp"

namespace CS460
{
    SpeedController::SpeedController()
    {
    }

    SpeedController::~SpeedController()
    {
    }

    Real SpeedController::DistanceTime(Real t) const
    {
        Real time = Math::Clamp(t, 0.0f, 1.0f);

        if (time < t1)
        {
            return S1(time);
        }
        if (t1 < time && time < t2)
        {
            return S2(time);
        }
        return S3(time);
    }

    void SpeedController::SetT1T2(Real _t1, Real _t2)
    {
        if (0.0f < _t1 && _t1 < _t2 && _t2 < 1.0f)
        {
            t1 = _t1;
            t2 = _t2;
            v0 = 2.0f / (1.0f - t1 + t2);
        }
    }

    Real SpeedController::S1(Real t) const
    {
        return 0.5f * v0 * t * t / t1;
    }

    Real SpeedController::S2(Real t) const
    {
        return v0 * (t - (t1 * 0.5f));
    }

    Real SpeedController::S3(Real t) const
    {
        return (v0 * (t - t2) * (2.0f - t - t2) / (2.0f * (1.0f - t2))) + (v0 * (t2 - (t1 * 0.5f)));
    }
}
