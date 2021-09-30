#pragma once
#include "../ComponentFactory.hpp"

namespace CS460
{
    class Component;
    class Object;

    class AniMeshFactory final : public ComponentFactory
    {
    public:
        AniMeshFactory();
        ~AniMeshFactory();

        Component* Create(Object* owner, Space* space) override;
        Component* Clone(Component* origin, Object* dest, Space* space) override;
    };
}
