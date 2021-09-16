#pragma once
#include <string>

namespace CS460
{
    class Component;
    class Object;
    class Space;

    class ComponentFactory
    {
    public:
        ComponentFactory()
        {
        }

        virtual ~ComponentFactory()
        {
        }

        virtual Component* Create(Object* owner, Space* space) = 0;
        virtual Component* Clone(Component* origin, Object* dest, Space* space) = 0;

    protected:
        friend class ComponentRegistry;
        std::string type;
    };
}
