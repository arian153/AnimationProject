#include "ControllerFactory.h"
#include "../../../Manager/Component/Component.hpp"
#include "../Component/ControllerComponent.h"

namespace Game
{
    ControllerFactory::ControllerFactory()
    {
        type = typeid(ControllerComponent).name();
    }

    ControllerFactory::~ControllerFactory()
    {
    }

    CS460::Component* ControllerFactory::Create(CS460::Object* owner, CS460::Space* space)
    {
        auto create    = new ControllerComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    CS460::Component* ControllerFactory::Clone(CS460::Component* origin, CS460::Object* dest, CS460::Space* space)
    {
        auto source = static_cast<ControllerComponent*>(origin);
        auto cloned = static_cast<ControllerComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
