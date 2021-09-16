#include "TransformFactory.hpp"
#include "../EngineComponent/TransformComponent.hpp"

namespace CS460
{
    TransformFactory::TransformFactory()
    {
        type = typeid(TransformComponent).name();
    }

    TransformFactory::~TransformFactory()
    {
    }

    Component* TransformFactory::Create(Object* owner, Space* space)
    {
        auto create = new TransformComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* TransformFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<TransformComponent*>(origin);
        auto cloned = static_cast<TransformComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
