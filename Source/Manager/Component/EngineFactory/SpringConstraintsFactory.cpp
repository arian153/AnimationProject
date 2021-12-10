#include "SpringConstraintsFactory.hpp"
#include "../EngineComponent/SpringConstraintsComponent.hpp"

namespace CS460
{
    SpringConstraintsFactory::SpringConstraintsFactory()
    {
        type = typeid(SpringConstraintsComponent).name();
    }

    SpringConstraintsFactory::~SpringConstraintsFactory()
    {
    }

    Component* SpringConstraintsFactory::Create(Object* owner, Space* space)
    {
        auto create = new SpringConstraintsComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* SpringConstraintsFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<SpringConstraintsComponent*>(origin);
        auto cloned = static_cast<SpringConstraintsComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
