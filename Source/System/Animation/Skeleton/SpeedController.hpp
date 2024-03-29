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
        Real InvDT(Real s) const;
        Real VT(Real t) const;

        void SetT1T2(Real _t1, Real _t2);

        Real T1() const;
        Real T2() const;
        Real V0() const;

    private:
        Real S1(Real t) const;
        Real S2(Real t) const;
        Real S3(Real t) const;

        Real InvS1(Real s) const;
        Real InvS2(Real s) const;
        Real InvS3(Real s) const;

    private:
        Real t1 = 0.0f;
        Real t2 = 0.0f;
        Real v0 = 1.0f;

    public:
        Real elapsed_s = 0.0f;
        Real elapsed_t = 0.0f;
    };
}
