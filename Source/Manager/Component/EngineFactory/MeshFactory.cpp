#include "MeshFactory.hpp"
#include "../EngineComponent/MeshComponent.hpp"

namespace CS460
{
    MeshFactory::MeshFactory()
    {
        type = typeid(MeshComponent).name();
    }

    MeshFactory::~MeshFactory()
    {
    }

    Component* MeshFactory::Create(Object* owner, Space* space)
    {
        auto create = new MeshComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* MeshFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<MeshComponent*>(origin);
        auto cloned = static_cast<MeshComponent*>(Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
