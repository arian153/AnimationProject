#include "ShaderResource.hpp"

namespace CS460
{
    ShaderResource::ShaderResource(const std::wstring& path)
        : Resource(path)
    {
        m_type = eResourceType::Shader;
    }

    ShaderResource::~ShaderResource()
    {
    }

    void ShaderResource::Initialize()
    {
        m_b_loaded = true;
    }

    void ShaderResource::Shutdown()
    {
        m_b_unloaded = true;
    }

    void ShaderResource::SetStage(eShaderStage stage)
    {
        m_stage = stage;
    }
}
