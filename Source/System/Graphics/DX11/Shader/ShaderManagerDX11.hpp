#pragma once
#include <d3d11.h>
#include <string>

namespace CS460
{
    class ShaderManagerDX11
    {
    public:
        ShaderManagerDX11();
        ~ShaderManagerDX11();

        void SetHWnd(HWND hwnd);
        void SetDevice(ID3D11Device* device);
        void SetDeviceContext(ID3D11DeviceContext* device_context);

        void OutputShaderErrorMessage(ID3D10Blob* error, HWND hwnd, const std::wstring& shader_name);

    protected:
        ID3D11DeviceContext* m_device_context = nullptr;
        ID3D11Device*        m_device         = nullptr;
        HWND                 m_hwnd           = nullptr;
    };
}
