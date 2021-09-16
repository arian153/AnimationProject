#pragma once

#include <vector>

#include "../../GraphicsAPI.hpp"
#include "../../DataType/VertexAttributeDef.hpp"

#include IncludeVertexLayoutAPI

namespace CS460
{
    class VertexAttributeCommon : public VertexAttributeAPI
    {
    public:
        VertexAttributeCommon()  = default;
        ~VertexAttributeCommon() = default;

        static U32 SizeOf(eAttributeType type);

    public:
        eAttributeType format_type  = eAttributeType::R32;
        U32            format_count = 0;
    };

    class VertexLayoutCommon final : public VertexLayoutAPI
    {
    public:
        VertexLayoutCommon();
        ~VertexLayoutCommon();

        void Push(const VertexAttributeCommon& attribute);
        void Clear();
        U32 GetStride() const;

        std::vector<VertexAttributeCommon>& GetAttributes();
        const std::vector<VertexAttributeCommon>& GetAttributes() const;

    private:
        friend class VertexLayoutAPI;

    private:
        std::vector<VertexAttributeCommon> m_attributes;
        U32                                m_stride = 0;
    };
}
