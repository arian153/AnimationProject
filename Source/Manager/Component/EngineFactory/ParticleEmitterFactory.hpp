#pragma once
#include "../ComponentFactory.hpp"

namespace CS460
{
    class Component;
    class Object;

    class ParticleEmitterFactory final : public ComponentFactory
    {
    public:
        ParticleEmitterFactory();
        ~ParticleEmitterFactory();

        Component* Create(Object* owner, Space* space) override;
        Component* Clone(Component* origin, Object* dest, Space* space) override;
    };
}
