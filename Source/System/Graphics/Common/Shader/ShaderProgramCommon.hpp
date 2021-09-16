#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeShaderProgramAPI

#include "../../../Math/Utility/MathDef.hpp"
#include "../../../Core/Utility/CoreDef.hpp"

namespace CS460
{
    class ConstantBufferCommon;
    class ShaderResource;
    class ShaderManagerCommon;
    class RendererCommon;
    class VertexLayoutCommon;

    class ShaderProgramCommon : public ShaderProgramAPI
    {
    public:
        explicit ShaderProgramCommon(ShaderManagerCommon* shader_manager);
        ~ShaderProgramCommon();

        bool Init(RendererCommon* renderer);
        void Shutdown();
        void Bind() const;

        void SetVertexLayout(VertexLayoutCommon* layout);
        void SetShaderResource(ShaderResource* resource);

    private:
        ShaderManagerCommon* m_shader_manager = nullptr;
        ShaderResource*      m_resource       = nullptr;
        VertexLayoutCommon*  m_vertex_layout  = nullptr;

        std::vector<ConstantBufferCommon*> m_constant_buffers;
    };
}
