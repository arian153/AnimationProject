#pragma warning( disable : 26812)

#include "RendererDX11.hpp"
#include "../../Core/Utility/CoreUtility.hpp"
#include "../../Math/Math.hpp"
#include "../DataType/Color.hpp"
#include <cassert>
#include "../Common/Renderer/RendererCommon.hpp"

namespace CS460
{
    RendererDX11::RendererDX11()
        : m_d3d_feature_level(),
          m_dxgi_color_format(DXGI_FORMAT_B8G8R8A8_UNORM),
          m_viewport(),
          m_numerator(0),
          m_denominator(0),
          m_video_card_memory(0),
          m_client_width(1280),
          m_client_height(720)
    {
    }

    RendererDX11::~RendererDX11()
    {
    }

    ID3D11Device* RendererDX11::GetDevice() const
    {
        return m_device;
    }

    ID3D11DeviceContext* RendererDX11::GetDeviceContext() const
    {
        return m_device_context;
    }

    ID3D11DepthStencilView* RendererDX11::GetDepthStencilView() const
    {
        return m_depth_stencil_view;
    }

    HWND RendererDX11::GetHwnd() const
    {
        return m_hwnd;
    }

    ID2D1DeviceContext1* RendererDX11::GetD2DDeviceContext() const
    {
        return m_d2d_device_context;
    }

    IDWriteFactory2* RendererDX11::GetDWWriteFactory() const
    {
        return m_write_factory;
    }

    ID3D11Texture2D* RendererDX11::GetFrameBuffer()
    {
        m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_frame_buffer));
        return m_frame_buffer;
    }

    String RendererDX11::GetVideoCardInfo(size_t& memory) const
    {
        memory = m_video_card_memory;
        return m_video_card_description;
    }

    void RendererDX11::SetHwnd(HWND hwnd)
    {
        m_hwnd = hwnd;
    }

    void RendererDX11::SetBackBufferRenderTarget() const
    {
        m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);
    }

    void RendererDX11::SetRasterStateWireFrame(bool flag) const
    {
        if (flag == true)
        {
            m_device_context->RSSetState(m_wire_frame_raster_state);
        }
        else
        {
            m_device_context->RSSetState(m_raster_state);
        }
    }

    void RendererDX11::ResetViewport() const
    {
        m_device_context->RSSetViewports(1, &m_viewport);
    }

    void RendererDX11::SetUpAdapterDescription(int client_width, int client_height)
    {
        IDXGIFactory*     factory;
        IDXGIAdapter*     adapter;
        IDXGIOutput*      adapter_output;
        UINT              num_modes = 0;
        DXGI_ADAPTER_DESC adapter_desc;
        // Create a DirectX graphics interface factory.
        HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
        if (FAILED(result))
            return;
        // Use the factory to create an adapter for the primary graphics interface (video card).
        result = factory->EnumAdapters(0, &adapter);
        if (FAILED(result))
            return;
        // Enumerate the primary adapter output (monitor).
        result = adapter->EnumOutputs(0, &adapter_output);
        if (FAILED(result))
            return;
        // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
        result = adapter_output->GetDisplayModeList(m_dxgi_color_format, DXGI_ENUM_MODES_INTERLACED, &num_modes, nullptr);
        if (FAILED(result))
            return;
        // Create a list to hold all the possible display modes for this monitor/video card combination.
        DXGI_MODE_DESC* display_mode_list = new DXGI_MODE_DESC[num_modes];
        if (!display_mode_list)
            return;
        // Now fill the display mode list structures.
        result = adapter_output->GetDisplayModeList(m_dxgi_color_format, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
        if (FAILED(result))
            return;
        // Now go through all the display modes and find the one that matches the screen width and height.
        // When a match is found store the numerator and denominator of the refresh rate for that monitor.
        for (size_t i = 0; i < num_modes; i++)
        {
            if (display_mode_list[i].Height == (UINT)client_height && display_mode_list[i].Width == (UINT)client_width)
            {
                m_numerator   = display_mode_list[i].RefreshRate.Numerator;
                m_denominator = display_mode_list[i].RefreshRate.Denominator;
            }
        }
        // Get the adapter (video card) description.
        result = adapter->GetDesc(&adapter_desc);
        if (FAILED(result))
            return;
        // Store the dedicated video card memory in megabytes.
        m_video_card_memory = (int)(adapter_desc.DedicatedVideoMemory / 1024 / 1024);
        //char video_card_description[128];
        //// Convert the name of the video card to a character array and store it.
        //error = wcstombs(video_card_description, adapter_desc.Description, 128);
        //
        //if (error != 0) return;
        m_video_card_description = ToString(std::wstring(adapter_desc.Description));
        // Release the display mode list.
        delete[] display_mode_list;
        display_mode_list = nullptr;
        // Release the adapter output.
        adapter_output->Release();
        adapter_output = nullptr;
        // Release the adapter.
        adapter->Release();
        adapter = nullptr;
        // Release the factory.
        factory->Release();
        factory = nullptr;
    }

    void RendererDX11::SetUpDevice()
    {
        UINT create_device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
        create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        HRESULT result = D3D11CreateDevice(
                                           nullptr, // default adapter
                                           D3D_DRIVER_TYPE_HARDWARE,
                                           nullptr,                 // no software device
                                           create_device_flags,
                                           nullptr, 0,              // default feature level array
                                           D3D11_SDK_VERSION,
                                           &m_device,
                                           &m_d3d_feature_level,
                                           &m_device_context);
        if (FAILED(result))
            return; //Failed Create D3D11 Device.
        if (m_d3d_feature_level != D3D_FEATURE_LEVEL_11_0)
        {
            MessageBox(nullptr, L"Direct3D Feature Level 11 unsupported.", nullptr, 0);
        }
    }

    void RendererDX11::SetUpSwapChain(int client_width, int client_height, bool fullscreen_flag)
    {
        DXGI_SWAP_CHAIN_DESC swap_chain_desc;
        // Initialize the swap chain description.
        ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
        // Set to a single back buffer.
        swap_chain_desc.BufferCount = m_back_buffer_count;
        // Set the width and height of the back buffer.
        swap_chain_desc.BufferDesc.Width  = client_width;
        swap_chain_desc.BufferDesc.Height = client_height;
        // Set regular 32-bit surface for the back buffer.
        swap_chain_desc.BufferDesc.Format = m_dxgi_color_format;
        // Set the refresh rate of the back buffer.
        if (m_vsync_enabled)
        {
            swap_chain_desc.BufferDesc.RefreshRate.Numerator   = m_numerator;
            swap_chain_desc.BufferDesc.RefreshRate.Denominator = m_denominator;
        }
        else
        {
            swap_chain_desc.BufferDesc.RefreshRate.Numerator   = 0;
            swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
        }
        // Set the usage of the back buffer.
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        // Set the handle for the window to render to.
        swap_chain_desc.OutputWindow       = m_hwnd;
        swap_chain_desc.SampleDesc.Count   = 1;
        swap_chain_desc.SampleDesc.Quality = 0;
        // Use 4X MSAA? Turn multi sampling on.
        if (m_enable_msaa)
        {
            swap_chain_desc.SampleDesc.Count   = 4;
            swap_chain_desc.SampleDesc.Quality = m_msaa_quality - 1;
        }
            // No MSAA Turn multi sampling off.
        else
        {
            swap_chain_desc.SampleDesc.Count   = 1;
            swap_chain_desc.SampleDesc.Quality = 0;
        }
        // Set to full screen or windowed mode.
        if (fullscreen_flag)
        {
            swap_chain_desc.Windowed = false;
        }
        else
        {
            swap_chain_desc.Windowed = true;
        }
        // Set the scan line ordering and scaling to unspecified.
        swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swap_chain_desc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
        // Discard the back buffer contents after presenting.
        swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        // Don't set the advanced flags.
        swap_chain_desc.Flags    = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        IDXGIDevice* dxgi_device = nullptr;
        HRESULT      result      = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device);
        if (FAILED(result))
            return;
        IDXGIAdapter* dxgi_adapter = nullptr;
        result                     = dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter);
        if (FAILED(result))
            return;
        IDXGIFactory* dxgi_factory = nullptr;
        result                     = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory);
        if (FAILED(result))
            return;
        result = dxgi_factory->CreateSwapChain(m_device, &swap_chain_desc, &m_swap_chain);
        if (FAILED(result))
            return;
        result = dxgi_factory->MakeWindowAssociation(m_hwnd, DXGI_MWA_NO_ALT_ENTER);
        if (FAILED(result))
            return;
        dxgi_device->Release();
        dxgi_device = nullptr;
        dxgi_adapter->Release();
        dxgi_adapter = nullptr;
        dxgi_factory->Release();
        dxgi_factory = nullptr;
        //// Create the swap chain, Direct3D device, and Direct3D device context.
        //HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &m_d3d_feature_level, 1,
        //	D3D11_SDK_VERSION, &swap_chain_desc, &m_swap_chain, &m_device, NULL, &m_device_context);
        //if (FAILED(result)) return;
    }

    void RendererDX11::SetUpBackBuffer()
    {
        ID3D11Texture2D* back_buffer;
        HRESULT          result = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
        if (FAILED(result))
            return;
        result = m_device->CreateRenderTargetView(back_buffer, nullptr, &m_render_target_view);
        if (FAILED(result))
            return;
        if (back_buffer != nullptr)
        {
            back_buffer->Release();
            back_buffer = nullptr;
        }
    }

    void RendererDX11::SetUpDepthBufferDescription(int client_width, int client_height)
    {
        D3D11_TEXTURE2D_DESC depth_buffer_desc;
        // Initialize the description of the depth buffer.
        ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));
        // Set up the description of the depth buffer.
        depth_buffer_desc.Width     = client_width;
        depth_buffer_desc.Height    = client_height;
        depth_buffer_desc.MipLevels = 1;
        depth_buffer_desc.ArraySize = 1;
        depth_buffer_desc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
        // Use 4X MSAA? --must match swap chain MSAA values.
        if (m_enable_msaa)
        {
            depth_buffer_desc.SampleDesc.Count   = 4;
            depth_buffer_desc.SampleDesc.Quality = m_msaa_quality - 1;
        }
            // No MSAA
        else
        {
            depth_buffer_desc.SampleDesc.Count   = 1;
            depth_buffer_desc.SampleDesc.Quality = 0;
        }
        depth_buffer_desc.Usage          = D3D11_USAGE_DEFAULT;
        depth_buffer_desc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
        depth_buffer_desc.CPUAccessFlags = 0;
        depth_buffer_desc.MiscFlags      = 0;
        // Create the texture for the depth buffer using the filled out description.
        HRESULT result = m_device->CreateTexture2D(&depth_buffer_desc, nullptr, &m_depth_stencil_buffer);
        if (FAILED(result))
        {
        }
    }

    void RendererDX11::SetUpStencilStateDescription()
    {
        D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
        // Initialize the description of the stencil state.
        ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
        // Set up the description of the stencil state.
        depth_stencil_desc.DepthEnable      = true;
        depth_stencil_desc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
        depth_stencil_desc.DepthFunc        = D3D11_COMPARISON_LESS;
        depth_stencil_desc.StencilEnable    = true;
        depth_stencil_desc.StencilReadMask  = 0xFF;
        depth_stencil_desc.StencilWriteMask = 0xFF;
        // Stencil operations if pixel is front-facing.
        depth_stencil_desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
        depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depth_stencil_desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
        depth_stencil_desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
        // Stencil operations if pixel is back-facing.
        depth_stencil_desc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
        depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        depth_stencil_desc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
        depth_stencil_desc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
        // Create the depth stencil state.
        HRESULT result = m_device->CreateDepthStencilState(&depth_stencil_desc, &m_depth_stencil_state);
        if (FAILED(result))
            return;
        // Set the depth stencil state.
        m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
        D3D11_DEPTH_STENCIL_DESC depth_disabled_stencil_desc;
        // Clear the second depth stencil state before setting the parameters.
        ZeroMemory(&depth_disabled_stencil_desc, sizeof(depth_disabled_stencil_desc));
        // Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
        // that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
        depth_disabled_stencil_desc.DepthEnable                  = false;
        depth_disabled_stencil_desc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
        depth_disabled_stencil_desc.DepthFunc                    = D3D11_COMPARISON_LESS;
        depth_disabled_stencil_desc.StencilEnable                = true;
        depth_disabled_stencil_desc.StencilReadMask              = 0xFF;
        depth_disabled_stencil_desc.StencilWriteMask             = 0xFF;
        depth_disabled_stencil_desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
        depth_disabled_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depth_disabled_stencil_desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
        depth_disabled_stencil_desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
        depth_disabled_stencil_desc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
        depth_disabled_stencil_desc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
        depth_disabled_stencil_desc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
        depth_disabled_stencil_desc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
        // Create the state using the device.
        result = m_device->CreateDepthStencilState(&depth_disabled_stencil_desc, &m_depth_disabled_stencil_state);
        if (FAILED(result))
        {
        }
    }

    void RendererDX11::SetUpDepthStencilViewDescription()
    {
        D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
        // Initialize the depth stencil view.
        ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));
        // Set up the depth stencil view description.
        depth_stencil_view_desc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_stencil_view_desc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
        depth_stencil_view_desc.Texture2D.MipSlice = 0;
        // Create the depth stencil view.
        HRESULT result = m_device->CreateDepthStencilView(m_depth_stencil_buffer, &depth_stencil_view_desc, &m_depth_stencil_view);
        if (FAILED(result))
            return;
        // Bind the render target view and depth stencil buffer to the output render pipeline.
        m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);
    }

    void RendererDX11::SetUpRasterDescription()
    {
        D3D11_RASTERIZER_DESC raster_desc;
        // Setup the raster description which will determine how and what polygons will be drawn.
        raster_desc.AntialiasedLineEnable = false;
        raster_desc.CullMode              = D3D11_CULL_BACK;
        raster_desc.DepthBias             = 0;
        raster_desc.DepthBiasClamp        = 0.0f;
        raster_desc.DepthClipEnable       = true;
        raster_desc.FillMode              = D3D11_FILL_SOLID;
        raster_desc.FrontCounterClockwise = false;
        raster_desc.MultisampleEnable     = false;
        raster_desc.ScissorEnable         = false;
        raster_desc.SlopeScaledDepthBias  = 0.0f;
        // Create the rasterizer state from the description we just filled out.
        HRESULT result = m_device->CreateRasterizerState(&raster_desc, &m_raster_state);
        if (FAILED(result))
            return;
        D3D11_RASTERIZER_DESC wire_frame_desc;
        // Setup the raster description which will determine how and what polygons will be drawn.
        wire_frame_desc.AntialiasedLineEnable = false;
        wire_frame_desc.CullMode              = D3D11_CULL_BACK;
        wire_frame_desc.DepthBias             = 0;
        wire_frame_desc.DepthBiasClamp        = 0.0f;
        wire_frame_desc.DepthClipEnable       = true;
        wire_frame_desc.FillMode              = D3D11_FILL_WIREFRAME;
        wire_frame_desc.FrontCounterClockwise = false;
        wire_frame_desc.MultisampleEnable     = false;
        wire_frame_desc.ScissorEnable         = false;
        wire_frame_desc.SlopeScaledDepthBias  = 0.0f;
        // Create the rasterizer state from the description we just filled out.
        result = m_device->CreateRasterizerState(&wire_frame_desc, &m_wire_frame_raster_state);
        if (FAILED(result))
            return;
        // Now set the rasterizer state.
        m_device_context->RSSetState(m_raster_state);
    }

    void RendererDX11::SetUpViewport(int client_width, int client_height)
    {
        // Setup the viewport for rendering.
        m_viewport.Width    = (Real)client_width;
        m_viewport.Height   = (Real)client_height;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_viewport.TopLeftX = 0.0f;
        m_viewport.TopLeftY = 0.0f;
        // Create the viewport.
        m_device_context->RSSetViewports(1, &m_viewport);
    }

    void RendererDX11::SetUpMultiSamplingLevel()
    {
        HRESULT result = m_device->CheckMultisampleQualityLevels(m_dxgi_color_format, 4, &m_msaa_quality);
        if (FAILED(result))
            return;
        assert(m_msaa_quality > 0);
    }

    void RendererDX11::SetUpBlendState()
    {
        D3D11_BLEND_DESC blend_state_description;
        ZeroMemory(&blend_state_description, sizeof(D3D11_BLEND_DESC));
        blend_state_description.RenderTarget[0].BlendEnable           = TRUE;
        blend_state_description.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
        blend_state_description.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
        blend_state_description.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
        blend_state_description.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
        blend_state_description.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
        blend_state_description.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
        blend_state_description.RenderTarget[0].RenderTargetWriteMask = 0x0f;
        //create blend state
        HRESULT result = m_device->CreateBlendState(&blend_state_description, &m_alpha_enabled_blending_state);
        if (FAILED(result))
            return;
        D3D11_BLEND_DESC blend_state_disable_description;
        ZeroMemory(&blend_state_disable_description, sizeof(D3D11_BLEND_DESC));
        blend_state_disable_description.RenderTarget[0].BlendEnable = FALSE;
        //create blend state
        result = m_device->CreateBlendState(&blend_state_disable_description, &m_alpha_disabled_blending_state);
        if (FAILED(result))
        {
        }
    }

    void RendererDX11::SetUpDWDevice()
    {
        //create D2D device, DW factory
        HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_d2d_factory);
        if (FAILED(result))
            return;
        result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown* *>(&m_write_factory));
        if (FAILED(result))
            return;
        IDXGIDevice* dxgi_device = nullptr;
        result                   = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device);
        if (FAILED(result))
            return;
        result = m_d2d_factory->CreateDevice(dxgi_device, &m_d2d_device);
        if (FAILED(result))
            return;
        m_d2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &m_d2d_device_context);
        if (FAILED(result))
            return;
        if (dxgi_device != nullptr)
        {
            dxgi_device->Release();
            dxgi_device = nullptr;
        }
    }

    void RendererDX11::SetUpDWRenderTarget()
    {
        D2D1_BITMAP_PROPERTIES1 bitmap_property =
                D2D1::BitmapProperties1(
                                        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                                        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
                                        m_dw_dpi, m_dw_dpi);
        // Direct2D needs the DXGI version of the back buffer
        IDXGISurface* dxgi_buffer = nullptr;
        HRESULT       result      = m_swap_chain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&dxgi_buffer));
        if (FAILED(result))
            return;
        // create the bitmap
        result = m_d2d_device_context->CreateBitmapFromDxgiSurface(dxgi_buffer, &bitmap_property, &m_target_bitmap);
        if (FAILED(result))
        {
            if (dxgi_buffer != nullptr)
            {
                dxgi_buffer->Release();
                dxgi_buffer = nullptr;
            }
            return;
        }
        //// set the newly created bitmap as render target
        m_d2d_device_context->SetTarget(m_target_bitmap);
        if (dxgi_buffer != nullptr)
        {
            dxgi_buffer->Release();
            dxgi_buffer = nullptr;
        }
        if (m_target_bitmap != nullptr)
        {
            m_target_bitmap->Release();
            m_target_bitmap = nullptr;
        }
    }

    RendererCommon::RendererCommon()
        : RendererDX11()
    {
    }

    RendererCommon::~RendererCommon()
    {
    }

    void RendererCommon::Initialize(int client_width, int client_height, bool b_fullscreen)
    {
        SetUpDevice();
        SetUpMultiSamplingLevel();
        SetUpAdapterDescription(client_width, client_height);
        SetUpSwapChain(client_width, client_height, b_fullscreen);
        SetUpBackBuffer();
        SetUpDepthBufferDescription(client_width, client_height);
        SetUpStencilStateDescription();
        SetUpDepthStencilViewDescription();
        SetUpRasterDescription();
        SetUpViewport(client_width, client_height);
        SetUpBlendState();
        SetUpDWDevice();
        SetUpDWRenderTarget();
        m_b_init = true;
    }

    void RendererCommon::Shutdown()
    {
        if (m_d2d_factory != nullptr)
        {
            m_d2d_factory->Release();
            m_d2d_factory = nullptr;
        }
        if (m_write_factory != nullptr)
        {
            m_write_factory->Release();
            m_write_factory = nullptr;
        }
        if (m_d2d_device != nullptr)
        {
            m_d2d_device->Release();
            m_d2d_device = nullptr;
        }
        if (m_d2d_device_context != nullptr)
        {
            m_d2d_device_context->Release();
            m_d2d_device_context = nullptr;
        }
        if (m_swap_chain != nullptr)
        {
            m_swap_chain->SetFullscreenState(false, nullptr);
        }
        if (m_alpha_enabled_blending_state != nullptr)
        {
            m_alpha_enabled_blending_state->Release();
            m_alpha_enabled_blending_state = nullptr;
        }
        if (m_alpha_disabled_blending_state != nullptr)
        {
            m_alpha_disabled_blending_state->Release();
            m_alpha_disabled_blending_state = nullptr;
        }
        if (m_raster_state != nullptr)
        {
            m_raster_state->Release();
            m_raster_state = nullptr;
        }
        if (m_wire_frame_raster_state != nullptr)
        {
            m_wire_frame_raster_state->Release();
            m_wire_frame_raster_state = nullptr;
        }
        if (m_depth_stencil_view != nullptr)
        {
            m_depth_stencil_view->Release();
            m_depth_stencil_view = nullptr;
        }
        if (m_depth_disabled_stencil_state != nullptr)
        {
            m_depth_disabled_stencil_state->Release();
            m_depth_disabled_stencil_state = nullptr;
        }
        if (m_depth_stencil_state != nullptr)
        {
            m_depth_stencil_state->Release();
            m_depth_stencil_state = nullptr;
        }
        if (m_depth_stencil_buffer != nullptr)
        {
            m_depth_stencil_buffer->Release();
            m_depth_stencil_buffer = nullptr;
        }
        if (m_render_target_view != nullptr)
        {
            m_render_target_view->Release();
            m_render_target_view = nullptr;
        }
        if (m_device_context != nullptr)
        {
            m_device_context->Release();
            m_device_context = nullptr;
        }
        if (m_device != nullptr)
        {
            m_device->Release();
            m_device = nullptr;
        }
        if (m_swap_chain != nullptr)
        {
            m_swap_chain->Release();
            m_swap_chain = nullptr;
        }
    }

    void RendererCommon::OnResize(int client_width, int client_height, bool b_fullscreen)
    {
        if (m_b_init == false)
        {
            return;
        }
        m_d2d_device_context->SetTarget(nullptr);
        m_device_context->ClearState();
        // do not call finish command list on state.
        //m_device_context->FinishCommandList(false, nullptr);
        if (m_render_target_view != nullptr)
        {
            m_render_target_view->Release();
            m_render_target_view = nullptr;
        }
        if (m_depth_stencil_view != nullptr)
        {
            m_depth_stencil_view->Release();
            m_depth_stencil_view = nullptr;
        }
        if (m_depth_stencil_buffer != nullptr)
        {
            m_depth_stencil_buffer->Release();
            m_depth_stencil_buffer = nullptr;
        }
        HRESULT result = m_swap_chain->ResizeBuffers(m_back_buffer_count, client_width, client_height, m_dxgi_color_format, 0);
        if (FAILED(result))
        {
            return;
        }
        OnFullscreen(b_fullscreen);
        SetUpBackBuffer();
        SetUpDepthBufferDescription(client_width, client_height);
        SetUpDepthStencilViewDescription();
        SetUpViewport(client_width, client_height);
        SetUpDWRenderTarget();
    }

    void RendererCommon::OnFullscreen(bool b_fullscreen) const
    {
        m_swap_chain->SetFullscreenState(b_fullscreen, nullptr);
    }

    void RendererCommon::BeginScene(Color color) const
    {
        Real color_arr[ 4 ];
        // Setup the color to clear the buffer to.
        color_arr[0] = color.r;
        color_arr[1] = color.g;
        color_arr[2] = color.b;
        color_arr[3] = color.a;
        // Clear the back buffer.
        if (m_render_target_view != nullptr)
        {
            m_device_context->ClearRenderTargetView(m_render_target_view, color_arr);
        }
        // Clear the depth buffer.
        if (m_depth_stencil_view != nullptr)
        {
            m_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
        }
        m_d2d_device_context->BeginDraw();
    }

    void RendererCommon::EndScene() const
    {
        m_d2d_device_context->EndDraw();
        if (m_vsync_enabled)
        {
            // Lock to screen refresh rate.
            m_swap_chain->Present(1, 0);
        }
        else
        {
            // Present as fast as possible.
            m_swap_chain->Present(0, 0);
        }
        // m_device_context->ClearState();
    }

    void RendererCommon::SetVSync(bool flag)
    {
        m_vsync_enabled = flag;
    }

    void RendererCommon::SetAlphaBlending(bool flag) const
    {
        Real blend_factor[ 4 ];
        // Setup the blend factor.
        blend_factor[0] = 0.0f;
        blend_factor[1] = 0.0f;
        blend_factor[2] = 0.0f;
        blend_factor[3] = 0.0f;
        // Turn on the alpha blending.
        if (flag == true)
        {
            m_device_context->OMSetBlendState(m_alpha_enabled_blending_state, blend_factor, 0xffffffff);
        }
        else
        {
            m_device_context->OMSetBlendState(m_alpha_disabled_blending_state, blend_factor, 0xffffffff);
        }
    }

    void RendererCommon::SetZBuffering(bool flag) const
    {
        if (flag == true)
        {
            m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
        }
        else
        {
            m_device_context->OMSetDepthStencilState(m_depth_disabled_stencil_state, 1);
        }
    }

    bool RendererCommon::IsInit() const
    {
        return m_b_init;
    }
}
