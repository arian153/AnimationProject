#pragma once
#include <vector>
#include <d3d11.h>

namespace CS460
{
    class TextureCommon;

    class TextureArrayDX11
    {
    public:
        TextureArrayDX11();
        ~TextureArrayDX11();

        ID3D11ShaderResourceView**       Data();
        ID3D11ShaderResourceView* const* Data() const;

    protected:
        std::vector<ID3D11ShaderResourceView*> m_textures;
        ID3D11DeviceContext*                   m_device_context = nullptr;
        TextureCommon*                         m_front          = nullptr;
    };
}
