#include "ColliderFactory.hpp"
#include "../EngineComponent/ColliderComponent.hpp"

namespace CS460
{
    ColliderFactory::ColliderFactory()
    {
        type = typeid(ColliderComponent).name();
    }

    ColliderFactory::~ColliderFactory()
    {
    }

    Component* ColliderFactory::Create(Object* owner, Space* space)
    {
        auto create = new ColliderComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* ColliderFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<ColliderComponent*>(origin);
        auto cloned = static_cast<ColliderComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
