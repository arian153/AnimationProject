#include "LightFactory.hpp"
#include "../EngineComponent/LightComponent.hpp"

namespace CS460
{
    LightFactory::LightFactory()
    {
        type = typeid(LightComponent).name();
    }

    LightFactory::~LightFactory()
    {
    }

    Component* LightFactory::Create(Object* owner, Space* space)
    {
        auto create = new LightComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* LightFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<LightComponent*>(origin);
        auto cloned = static_cast<LightComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
