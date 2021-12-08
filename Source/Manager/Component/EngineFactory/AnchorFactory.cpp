#include "AnchorFactory.hpp"
#include "../EngineComponent/AnchorComponent.hpp"

namespace CS460
{
    AnchorFactory::AnchorFactory()
    {
        type = typeid(AnchorComponent).name();
    }

    AnchorFactory::~AnchorFactory()
    {
    }

    Component* AnchorFactory::Create(Object* owner, Space* space)
    {
        auto create    = new AnchorComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* AnchorFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<AnchorComponent*>(origin);
        auto cloned = static_cast<AnchorComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
