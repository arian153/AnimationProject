#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeVertexBufferAPI
#include "../../DataType/TopologyDef.hpp"
#include <vector>

#include "../../../Core/Utility/CoreDef.hpp"
#include "../Vertex/ColorVertexCommon.hpp"
#include "../Vertex/NormalVertexCommon.hpp"
#include "../Vertex/VertexCommon.hpp"
#include "../Vertex/TextureVertexCommon.hpp"

namespace CS460
{
    class InstanceBufferCommon;
    class RendererCommon;

    class VertexBufferCommon : public VertexBufferAPI
    {
    public:
        VertexBufferCommon();
        ~VertexBufferCommon();

        bool Init(RendererCommon* renderer, const std::vector<ColorVertexCommon>& vertices, bool is_dynamic = false);
        bool Init(RendererCommon* renderer, const std::vector<TextureVertexCommon>& vertices, bool is_dynamic = false);
        bool Init(RendererCommon* renderer, const std::vector<VertexCommon>& vertices, bool is_dynamic = false);
        bool Init(RendererCommon* renderer, const std::vector<NormalVertexCommon>& vertices, bool is_dynamic = false);
        bool Init(RendererCommon* renderer, const std::vector<Vector3>& vertices, bool is_dynamic = false);

        void Update(const std::vector<ColorVertexCommon>& vertices) const;
        void Update(const std::vector<TextureVertexCommon>& vertices) const;
        void Update(const std::vector<VertexCommon>& vertices) const;
        void Update(const std::vector<NormalVertexCommon>& vertices) const;
        void Update(const std::vector<Vector3>& vertices) const;

        bool Init(RendererCommon* renderer, void* data, size_t vertex_size, size_t vertex_count, bool is_dynamic = false);
        void Update(void* data, size_t vertex_count) const;
        void Bind(U32 offset) const;
        void Bind(U32 offset, InstanceBufferCommon* instance_buffer) const;
        void Shutdown();
        void SetPrimitiveTopology(eTopologyType type);

    private:
        U32  m_stride    = 0;
        bool m_b_dynamic = false;
    };
}
