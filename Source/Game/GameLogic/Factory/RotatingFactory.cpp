#include "RotatingFactory.h"
#include "../../../Manager/Component/Component.hpp"
#include "../Component/RotatingComponent.h"

namespace Game
{
    RotatingFactory::RotatingFactory()
    {
        type = typeid(RotatingComponent).name();
    }

    RotatingFactory::~RotatingFactory()
    {
    }

    CS460::Component* RotatingFactory::Create(CS460::Object* owner, CS460::Space* space)
    {
        auto create = new RotatingComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    CS460::Component* RotatingFactory::Clone(CS460::Component* origin, CS460::Object* dest, CS460::Space* space)
    {
        auto source = static_cast<RotatingComponent*>(origin);
        auto cloned = static_cast<RotatingComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
