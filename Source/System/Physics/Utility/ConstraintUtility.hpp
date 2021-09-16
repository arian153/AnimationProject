#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    enum class eConstraintMode
    {
        Hard
      , Soft
    };

    class ConstraintCoefficient
    {
    public:
        Real damping_coefficient = 0.0f;
        Real spring_constant     = 0.0f;
    };

    class ConstraintBias
    {
    public:
        Real position_bias = 0.0f;
        Real softness_bias = 0.0f;
    };

    class ConstraintUtility
    {
    public:
        ConstraintUtility();
        ~ConstraintUtility();

        ConstraintCoefficient GenerateSoftCoefficient(Real mass, Real frequency, Real damping_ratio);
        ConstraintBias        GenerateConstraintBias(Real damping_coefficient, Real spring_constant, eConstraintMode mode, Real dt);
        ConstraintBias        GenerateConstraintBias(const ConstraintCoefficient& c, eConstraintMode mode, Real dt);
        ConstraintBias        GenerateConstraintBias(Real mass, Real frequency, Real damping_ratio, eConstraintMode mode, Real dt);

        Real GenerateDampingCoefficient(Real mass, Real frequency, Real damping_ratio);
        Real GenerateSpringConstant(Real mass, Real frequency);

    private:
    };
}
