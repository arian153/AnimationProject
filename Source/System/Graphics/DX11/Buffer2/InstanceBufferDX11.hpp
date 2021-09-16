#pragma once
#include <d3d11.h>

#include "../../../Core/Utility/CoreDef.hpp"

namespace CS460
{
    class InstanceBufferDX11
    {
    public:
        InstanceBufferDX11();
        ~InstanceBufferDX11();

    public:
        ID3D11Buffer* m_instance_buffer = nullptr;
        U32           m_stride          = 0;
    protected:
        ID3D11DeviceContext* m_device_context = nullptr;
        ID3D11Device*        m_device         = nullptr;
    };
}
