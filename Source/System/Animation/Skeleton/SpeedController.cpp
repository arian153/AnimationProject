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

    Real SpeedController::InvDT(Real s) const
    {
        Real clamped_s = Math::Clamp(s, 0.0f, 1.0f);

        if (clamped_s < S1(t1))
        {
            return InvS1(clamped_s);
        }
        if (S1(t1) < clamped_s && clamped_s < S2(t2))
        {
            return InvS2(clamped_s);
        }
        return InvS3(clamped_s);
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

    Real SpeedController::T1() const
    {
        return t1;
    }

    Real SpeedController::T2() const
    {
        return t2;
    }

    Real SpeedController::V0() const
    {
        return v0;
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

    Real SpeedController::InvS1(Real s) const
    {
        return sqrtf(2.0f * t1 * s);
    }

    Real SpeedController::InvS2(Real s) const
    {
        return (s / v0) + (t1 * 0.5f);
    }

    Real SpeedController::InvS3(Real s) const
    {
        Real s2_t2 = v0 * (t2 - (t1 * 0.5f));
        Real c1    = 2.0f * (s - s2_t2) * (1.0f - t2);
        Real c2    = t2 * t2 - (2.0f * t2);
        Real c     = c1 - c2;

        Real d = sqrtf(1.0f - c);
        return 1.0f - d;
    }
}
