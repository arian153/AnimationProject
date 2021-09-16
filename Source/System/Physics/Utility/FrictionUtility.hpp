#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include <unordered_map>
#include "MaterialDef.hpp"

namespace CS460
{
       class FrictionCoefficient
    {
    public:
        explicit FrictionCoefficient(Real static_friction = 0.5f, Real dynamic_friction = 0.4f);
        ~FrictionCoefficient();

    public:
        Real static_friction;
        Real dynamic_friction;
    };

    class FrictionUtility
    {
    public:
        FrictionUtility();
        ~FrictionUtility();

        void Initialize();
        void Shutdown();

        //find
        FrictionCoefficient Find(Physics::eMaterial a, Physics::eMaterial b);
        FrictionCoefficient Find(Physics::MaterialData a, Physics::MaterialData b);

    private:
        std::unordered_map<size_t, FrictionCoefficient> m_friction_map;
    };
}
