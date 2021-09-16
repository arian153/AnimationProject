#include "SkyFactory.hpp"
#include "../EngineComponent/SkyComponent.hpp"

namespace CS460
{
    SkyFactory::SkyFactory()
    {
        type = typeid(SkyComponent).name();
    }

    SkyFactory::~SkyFactory()
    {
    }

    Component* SkyFactory::Create(Object* owner, Space* space)
    {
        auto create    = new SkyComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* SkyFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<SkyComponent*>(origin);
        auto cloned = static_cast<SkyComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
