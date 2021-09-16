#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeRendererAPI

namespace CS460
{
    class MeshBufferCommon;
    class ColorShaderCommon;

    class RendererCommon : public RendererAPI
    {
    public:
        RendererCommon();
        ~RendererCommon();

        void Initialize(int client_width, int client_height, bool b_fullscreen);
        void Shutdown();
        void OnResize(int client_width, int client_height, bool b_fullscreen);
        void OnFullscreen(bool b_fullscreen) const;
        void BeginScene(Color color) const;
        void EndScene() const;

        //setter
        void SetVSync(bool flag);
        void SetAlphaBlending(bool flag) const;
        void SetZBuffering(bool flag) const;
        bool IsInit() const;
    private:
        bool m_b_init = false;
    };
}
