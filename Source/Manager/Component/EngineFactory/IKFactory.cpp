#include "IKFactory.hpp"
#include "../EngineComponent/IKComponent.hpp"

namespace CS460
{
    IKFactory::IKFactory()
    {
        type = typeid(IKComponent).name();
    }

    IKFactory::~IKFactory()
    {
    }

    Component* IKFactory::Create(Object* owner, Space* space)
    {
        auto create = new IKComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* IKFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<IKComponent*>(origin);
        auto cloned = static_cast<IKComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
