#pragma once
#include "../../../Manager/Component/ComponentFactory.hpp"

namespace Game
{
    class ControllerFactory final : public CS460::ComponentFactory
    {
    public:
        ControllerFactory();
        ~ControllerFactory();

        CS460::Component* Create(CS460::Object* owner, CS460::Space* space) override;
        CS460::Component* Clone(CS460::Component* origin, CS460::Object* dest, CS460::Space* space) override;
    };
}
