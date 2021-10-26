#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class SpeedController
    {
    public:
        SpeedController();
        ~SpeedController();

        Real DistanceTime(Real t) const;

        void SetT1T2(Real _t1, Real _t2);

    private:
        Real S1(Real t) const;
        Real S2(Real t) const;
        Real S3(Real t) const;

    private:
        Real t1 = 0.0f;
        Real t2 = 0.0f;
        Real v0 = 0.0f;
    };
}
