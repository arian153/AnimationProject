#pragma once
#include "../ComponentFactory.hpp"

namespace CS460
{
    class Component;
    class Object;

    class SpringConstraintsFactory final : public ComponentFactory
    {
    public:
        SpringConstraintsFactory();
        ~SpringConstraintsFactory();

        Component* Create(Object* owner, Space* space) override;
        Component* Clone(Component* origin, Object* dest, Space* space) override;
    };
}
