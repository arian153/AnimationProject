#include "AniMeshFactory.hpp"
#include "../EngineComponent/AniMeshComponent.hpp"

namespace CS460
{
    AniMeshFactory::AniMeshFactory()
    {
        type = typeid(AniMeshComponent).name();
    }

    AniMeshFactory::~AniMeshFactory()
    {
    }

    Component* AniMeshFactory::Create(Object* owner, Space* space)
    {
        auto create = new AniMeshComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* AniMeshFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<AniMeshComponent*>(origin);
        auto cloned = static_cast<AniMeshComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
