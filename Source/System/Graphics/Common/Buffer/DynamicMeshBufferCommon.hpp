#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeDynamicMeshBufferAPI
#include <vector>

#include "../../../Core/Utility/CoreDef.hpp"
#include "../Vertex/VertexCommon.hpp"
#include "../Vertex/ColorVertexCommon.hpp"
#include "../Vertex/TextureVertexCommon.hpp"

namespace CS460
{
    class RendererCommon;

    class DynamicMeshBufferCommon : public DynamicMeshBufferAPI
    {
    public:
        DynamicMeshBufferCommon();
        ~DynamicMeshBufferCommon();

        void Render(U32 stride, U32 offset) const;
        void Shutdown();

        bool BuildBuffer(RendererCommon* renderer, const std::vector<ColorVertexCommon>& vertices, const std::vector<U32>& indices);
        bool BuildBuffer(RendererCommon* renderer, const std::vector<TextureVertexCommon>& vertices, const std::vector<U32>& indices);
        bool BuildBuffer(RendererCommon* renderer, const std::vector<VertexCommon>& vertices, const std::vector<U32>& indices);

        void UpdateVertexBuffer(const std::vector<ColorVertexCommon>& vertices) const;
        void UpdateVertexBuffer(const std::vector<TextureVertexCommon>& vertices) const;
        void UpdateVertexBuffer(const std::vector<VertexCommon>& vertices) const;

    private:
    };
}
