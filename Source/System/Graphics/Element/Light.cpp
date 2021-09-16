#include "Light.hpp"

#include "../../../Manager/Component/EngineComponent/LightComponent.hpp"

namespace CS460
{
    Light::Light()
    {
    }

    Light::~Light()
    {
    }

    void Light::Initialize()
    {
    }

    void Light::Update(Real dt) const
    {
        if (m_component != nullptr)
        {
            m_component->Update(dt);
        }
    }

    void Light::Shutdown() const
    {
        if (m_component != nullptr)
        {
            m_component->m_light = nullptr;
        }
    }

    LightBufferData Light::GetLightBuffer() const
    {
        return light_data;
    }

    void Light::SetType(eLightType type)
    {
        m_type          = type;
        light_data.type = (int)type;
    }
}
