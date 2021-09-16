#include "TextureDX11.hpp"
#include "../../Common/Texture/TextureCommon.hpp"
#include "../../../../External/DirectXTextureLoader/DDSTextureLoader.h"
#include "../../../../External/DirectXTextureLoader/WICTextureLoader.h"
#include "../../Common/Renderer/RendererCommon.hpp"
#include "../../Common/Buffer/TextTextureBufferCommon.hpp"
#include "../../Common/Buffer/RenderTextureBufferCommon.hpp"

namespace CS460
{
    TextureDX11::TextureDX11()
    {
    }

    TextureDX11::~TextureDX11()
    {
    }

    ID3D11ShaderResourceView* TextureDX11::GetTexture() const
    {
        return m_texture;
    }

    TextureCommon::TextureCommon()
    {
    }

    TextureCommon::~TextureCommon()
    {
    }

    bool TextureCommon::Initialize(RendererCommon* renderer, const std::wstring& file_path, const std::wstring& file_type)
    {
        HRESULT result;
        if (file_type == L".dds" || file_type == L".DDS")
        {
            result = DirectX::CreateDDSTextureFromFile
                    (
                     renderer->GetDevice(),
                     renderer->GetDeviceContext(),
                     file_path.c_str(),
                     &m_texture_resource,
                     &m_texture
                    );
        }
        else if (
            file_type == L".png" || file_type == L".PNG" ||
            file_type == L".jpeg" || file_type == L".JPEG" ||
            file_type == L".jpg" || file_type == L".JPG" ||
            file_type == L".bmp" || file_type == L".BMP" ||
            file_type == L".tiff" || file_type == L".TIFF" ||
            file_type == L".gif" || file_type == L".GIF")
        {
            result = DirectX::CreateWICTextureFromFile
                    (
                     renderer->GetDevice(),
                     renderer->GetDeviceContext(),
                     file_path.c_str(),
                     &m_texture_resource,
                     &m_texture
                    );
        }
        else
        {
            return InitializeDefault(renderer);
        }
        if (FAILED(result))
        {
            return false;
        }
        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    bool TextureCommon::Initialize(TextTextureBufferCommon* text_buffer)
    {
        m_texture   = text_buffer->GetTextResource();
        m_b_created = true;
        return true;
    }

    bool TextureCommon::Initialize(RenderTextureBufferCommon* render_texture_buffer)
    {
        m_texture        = render_texture_buffer->GetTextureResource();
        m_device_context = render_texture_buffer->GetDeviceContext();
        m_b_created      = true;
        return true;
    }

    bool TextureCommon::InitializeDefault(RendererCommon* renderer)
    {
        static const uint32_t s_pixel = 0xffffffff;

        D3D11_SUBRESOURCE_DATA initData = {&s_pixel, sizeof(uint32_t), 0};

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width                = desc.Height = desc.MipLevels = desc.ArraySize = 1;
        desc.Format               = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count     = 1;
        desc.Usage                = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags            = D3D11_BIND_SHADER_RESOURCE;

        ID3D11Texture2D* tex;
        auto             device  = renderer->GetDevice();
        HRESULT          hresult = device->CreateTexture2D(&desc, &initData, &tex);

        if (SUCCEEDED(hresult))
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc = {};
            shader_resource_view_desc.Format                          = DXGI_FORMAT_R8G8B8A8_UNORM;
            shader_resource_view_desc.ViewDimension                   = D3D11_SRV_DIMENSION_TEXTURE2D;
            shader_resource_view_desc.Texture2D.MipLevels             = 1;

            hresult = device->CreateShaderResourceView(tex, &shader_resource_view_desc, &m_texture);
        }

        if (FAILED(hresult))
        {
            return false;
        }

        if (tex != nullptr)
        {
            tex->Release();
            tex = nullptr;
        }

        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    bool TextureCommon::Shutdown()
    {
        if (m_b_created == false)
        {
            if (m_texture != nullptr)
            {
                m_texture->Release();
                m_texture = nullptr;
            }
            if (m_texture_resource != nullptr)
            {
                m_texture_resource->Release();
                m_texture_resource = nullptr;
            }
        }
        return true;
    }

    void TextureCommon::Bind() const
    {
        m_device_context->PSSetShaderResources(0, 1, &m_texture);
    }
}
