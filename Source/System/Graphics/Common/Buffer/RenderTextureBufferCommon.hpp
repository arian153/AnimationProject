#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeRenderTextureBufferAPI
#include "../../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class RendererCommon;

    class RenderTextureBufferCommon : public RenderTextureBufferAPI
    {
    public:
        RenderTextureBufferCommon();
        ~RenderTextureBufferCommon();

        bool Initialize(RendererCommon* renderer, size_t width, size_t height);
        void Render() const;
        void Shutdown();
        bool OnResize(size_t width, size_t height) ;
        U32  GetIndexCount() const;
    private:
    };
}
