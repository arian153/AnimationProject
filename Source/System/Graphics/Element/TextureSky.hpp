#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class RendererCommon;
    class VertexBufferCommon;
    class IndexBufferCommon;
    class Camera;
    class TextureCommon;
    class MeshData;

    class TextureSky
    {
    public:
        TextureSky();
        ~TextureSky();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

    private:
        friend class SkyComponent;

    private:
        SkyComponent* m_component     = nullptr;
        RendererCommon*          m_renderer      = nullptr;
        VertexBufferCommon*      m_vertex_buffer = nullptr;
        IndexBufferCommon*       m_index_buffer  = nullptr;
        TextureCommon*           m_texture       = nullptr;
        Camera*                  m_camera        = nullptr;
    };
}
