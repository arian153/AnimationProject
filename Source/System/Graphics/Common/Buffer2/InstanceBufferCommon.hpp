#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeInstanceBufferAPI
#include "../../../Core/Utility/CoreDef.hpp"
#include <vector>

#include "ConstantBufferData.hpp"

namespace CS460
{
    class RendererCommon;

    class InstanceBufferCommon : public InstanceBufferAPI
    {
    public:
        InstanceBufferCommon();
        ~InstanceBufferCommon();

        bool Init(RendererCommon* renderer, const std::vector<InstanceBufferData>& instances);
        void Update(const std::vector<InstanceBufferData>& instances) const;
        void Shutdown();
    private:
    };
}
