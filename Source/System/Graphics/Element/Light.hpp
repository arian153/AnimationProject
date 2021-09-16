#pragma once
#include "LightDef.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../Common/Buffer2/ConstantBufferData.hpp"
#include "../DataType/Color.hpp"

namespace CS460
{
    class LightComponent;

    class Light
    {
    public:
        Light();
        ~Light();

        void Initialize();
        void Update(Real dt) const;
        void Shutdown() const;

        LightBufferData GetLightBuffer() const;
        void            SetType(eLightType type);

    public:
        LightBufferData light_data;

    private:
        friend class LightComponent;
    private:
        LightComponent* m_component = nullptr;
        eLightType      m_type      = eLightType::PointLight;
    };
}
