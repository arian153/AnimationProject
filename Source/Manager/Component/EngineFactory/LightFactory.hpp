#pragma once
#include "../ComponentFactory.hpp"

namespace CS460
{
    class Component;
    class Object;

    class LightFactory final : public ComponentFactory
    {
    public:
        LightFactory();
        ~LightFactory();

        Component* Create(Object* owner, Space* space) override;
        Component* Clone(Component* origin, Object* dest, Space* space) override;
    };
}
