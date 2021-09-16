#include "CameraFactory.hpp"
#include "../EngineComponent/CameraComponent.hpp"

namespace CS460
{
    CameraFactory::CameraFactory()
    {
        type = typeid(CameraComponent).name();
    }

    CameraFactory::~CameraFactory()
    {
    }

    Component* CameraFactory::Create(Object* owner, Space* space)
    {
        auto create = new CameraComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* CameraFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<CameraComponent*>(origin);
        auto cloned = static_cast<CameraComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
