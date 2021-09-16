#include "ConstraintUtility.hpp"

namespace CS460
{
    ConstraintUtility::ConstraintUtility()
    {
    }

    ConstraintUtility::~ConstraintUtility()
    {
    }

    ConstraintCoefficient ConstraintUtility::GenerateSoftCoefficient(Real mass, Real frequency, Real damping_ratio)
    {
        Real angular_frequency = frequency * Math::TWO_PI;
        //result
        ConstraintCoefficient result;
        result.damping_coefficient = 2.0f * mass * damping_ratio * angular_frequency;
        result.spring_constant     = mass * angular_frequency * angular_frequency;
        return result;
    }

    ConstraintBias ConstraintUtility::GenerateConstraintBias(Real damping_coefficient, Real spring_constant, eConstraintMode mode, Real dt)
    {
        if (mode == eConstraintMode::Hard)
        {
            ConstraintBias result;
            result.position_bias = 1.0f / dt;
            result.softness_bias = 0.0f;
            return result;
        }
        Real hk     = dt * spring_constant;
        Real gamma  = damping_coefficient + hk;
        Real inv_dt = 1.0f / dt;
        gamma       = gamma > 0.0f ? 1.0f / gamma : gamma;
        ConstraintBias result;
        result.position_bias = hk * gamma * inv_dt;
        result.softness_bias = gamma * inv_dt;
        return result;
    }

    ConstraintBias ConstraintUtility::GenerateConstraintBias(const ConstraintCoefficient& c, eConstraintMode mode, Real dt)
    {
        return GenerateConstraintBias(c.damping_coefficient, c.spring_constant, mode, dt);
    }

    ConstraintBias ConstraintUtility::GenerateConstraintBias(Real mass, Real frequency, Real damping_ratio, eConstraintMode mode, Real dt)
    {
        Real damping_coefficient = GenerateDampingCoefficient(mass, frequency, damping_ratio);
        Real spring_constant     = GenerateSpringConstant(mass, frequency);
        return GenerateConstraintBias(damping_coefficient, spring_constant, mode, dt);
    }

    Real ConstraintUtility::GenerateDampingCoefficient(Real mass, Real frequency, Real damping_ratio)
    {
        return 2.0f * mass * damping_ratio * frequency * Math::TWO_PI;
    }

    Real ConstraintUtility::GenerateSpringConstant(Real mass, Real frequency)
    {
        Real angular_frequency = frequency * Math::TWO_PI;
        return mass * angular_frequency * angular_frequency;
    }
}
