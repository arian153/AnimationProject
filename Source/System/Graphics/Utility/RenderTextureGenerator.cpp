#include "RenderTextureGenerator.hpp"
#include "../Common/Buffer/RenderTextureBufferCommon.hpp"
#include "MatrixManager.hpp"
#include "../Common/Texture/TextureCommon.hpp"
#include "../Common/Renderer/RendererCommon.hpp"
#include "../Element/Scene.hpp"
#include "../Common/Shader/ShaderManagerCommon.hpp"
#include "../../../Manager/Space/Space.hpp"

namespace CS460
{
    RenderTextureGenerator::RenderTextureGenerator()
    {
    }

    RenderTextureGenerator::~RenderTextureGenerator()
    {
    }

    void RenderTextureGenerator::Initialize(RendererCommon* renderer, MatrixManager* matrix_manager, ShaderManagerCommon* shader_manager)
    {
        m_renderer       = renderer;
        m_matrix_manager = matrix_manager;
        m_shader_manager = shader_manager;
        m_width          = m_matrix_manager->GetScreenWidth();
        m_height         = m_matrix_manager->GetScreenHeight();
        m_render_texture = new RenderTextureBufferCommon();
        m_render_texture->Initialize(renderer, m_width, m_height);
        m_texture = new TextureCommon();
    }

    void RenderTextureGenerator::Shutdown()
    {
        if (m_render_texture != nullptr)
        {
            m_render_texture->Shutdown();
            delete m_render_texture;
            m_render_texture = nullptr;
        }
        if (m_texture != nullptr)
        {
            delete m_texture;
            m_texture = nullptr;
        }
    }

    void RenderTextureGenerator::Render(Space* space)
    {
        space->Render();
    }

    void RenderTextureGenerator::Render(Scene* scene)
    {
        scene->Render();
    }

    void RenderTextureGenerator::BeginRenderToTexture(const Color& color) const
    {
        m_render_texture->SetRenderTarget();
        m_render_texture->ClearRenderTarget(color);
    }

    void RenderTextureGenerator::EndRenderToTexture() const
    {
        m_renderer->SetBackBufferRenderTarget();
    }

    void RenderTextureGenerator::SetTextureSize(size_t width, size_t height)
    {
        m_width  = width;
        m_height = height;
        ResizeTexture();
    }

    void RenderTextureGenerator::SetTextureScreenSize()
    {
        m_b_sync_screen = true;
        m_width         = m_matrix_manager->GetScreenWidth();
        m_height        = m_matrix_manager->GetScreenHeight();
        ResizeTexture();
    }

    void RenderTextureGenerator::OnResize()
    {
        if (m_b_sync_screen == true)
        {
            m_width  = m_matrix_manager->GetScreenWidth();
            m_height = m_matrix_manager->GetScreenHeight();
            ResizeTexture();
        }
    }

    TextureCommon* RenderTextureGenerator::GetTexture() const
    {
        m_texture->Initialize(m_render_texture);
        return m_texture;
    }

    void RenderTextureGenerator::ResizeTexture() const
    {
        m_render_texture->OnResize(m_width, m_height);
    }
}
