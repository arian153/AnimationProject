#pragma once
//#include <unordered_map>

#include <unordered_map>

#include "../../GraphicsAPI.hpp"
#include "../../../Core/Utility/CoreDef.hpp"

#include IncludeShaderManagerAPI
#include "../../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class TextureShaderCommon;
    class ConstantBufferCommon;
    class VertexLayoutCommon;
    class ShaderProgramCommon;
    class DirectionalLight;
    class Camera;
    class TextureCommon;
    class MatrixData;
    class ResourceManager;
    class RendererCommon;
    class Color;
    class TextureArrayCommon;

    class ShaderManagerCommon : public ShaderManagerAPI
    {
    public:
        ShaderManagerCommon();
        ~ShaderManagerCommon();
        void Initialize(RendererCommon* renderer, ResourceManager* resource_manager);
        void Shutdown();

        void                 AddShader(const std::wstring& name, VertexLayoutCommon* layout);
        void                 Bind(const std::string& name);
        ShaderProgramCommon* GetShader(const std::string& name) const;

        //temp
    private:
        //shader tables
        std::unordered_map<std::string, ShaderProgramCommon*> m_shader_table;

        //vertex layouts
        VertexLayoutCommon* m_color_vertex_layout              = nullptr;
        VertexLayoutCommon* m_instancing_color_vertex_layout   = nullptr;
        VertexLayoutCommon* m_texture_vertex_layout            = nullptr;
        VertexLayoutCommon* m_instancing_texture_vertex_layout = nullptr;
        VertexLayoutCommon* m_normal_vertex_layout             = nullptr;
        VertexLayoutCommon* m_instancing_normal_vertex_layout  = nullptr;
        VertexLayoutCommon* m_vertex_layout                    = nullptr;
        VertexLayoutCommon* m_instancing_vertex_layout         = nullptr;
        VertexLayoutCommon* m_point_vertex_layout              = nullptr;
        VertexLayoutCommon* m_skinned_vertex_layout            = nullptr;

        //others
        RendererCommon*  m_renderer         = nullptr;
        ResourceManager* m_resource_manager = nullptr;
    };
}
