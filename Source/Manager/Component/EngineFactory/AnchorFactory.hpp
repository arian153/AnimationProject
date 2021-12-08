#pragma once
#include "../ComponentFactory.hpp"

namespace CS460
{
    class Component;
    class Object;

    class AnchorFactory final : public ComponentFactory
    {
    public:
        AnchorFactory();
        ~AnchorFactory();

        Component* Create(Object* owner, Space* space) override;
        Component* Clone(Component* origin, Object* dest, Space* space) override;
    };
}
