#pragma once

#include <d3d11.h>
#include <string>

namespace CS460
{
    class ShaderProgramDX11
    {
    public:
        ShaderProgramDX11();
        ~ShaderProgramDX11();

    protected:
        ID3D11VertexShader*  m_vertex_shader  = nullptr;
        ID3D11PixelShader*   m_pixel_shader   = nullptr;

        ID3D11InputLayout*   m_layout         = nullptr;
        ID3D11SamplerState*  m_sampler_state  = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;
        ID3D11Device*        m_device         = nullptr;
        HWND                 m_hwnd           = nullptr;
    };
}
