#include "VertexLayoutDX11.hpp"

#include "../../../Core/Utility/CoreDef.hpp"
#include "../../Common/Buffer2/VertexLayoutCommon.hpp"

namespace CS460
{
    U32 VertexAttributeCommon::SizeOf(eAttributeType type)
    {
        switch (type)
        {
        case eAttributeType::T32:
            return sizeof(int);
        case eAttributeType::R32:
            return sizeof(R32);
        case eAttributeType::I32:
            return sizeof(I32);
        case eAttributeType::U32:
            return sizeof(U32);
        default:
            return 0;
        }
    }

    VertexLayoutDX11::VertexLayoutDX11()
    {
    }

    VertexLayoutDX11::~VertexLayoutDX11()
    {
    }

    void VertexLayoutDX11::PushDX11(eAttributeType type, U32 count, const std::string& semantic_name, U32 semantic_idx)
    {
        VertexLayoutCommon* layout = (VertexLayoutCommon*)this;

        VertexAttributeCommon attribute;
        attribute.format_type  = type;
        attribute.format_count = count;
        attribute.sem_name     = semantic_name;
        attribute.sem_idx      = semantic_idx;

        layout->m_attributes.push_back(attribute);
        layout->m_stride += VertexAttributeCommon::SizeOf(type) * count;
    }

    void VertexLayoutDX11::PushDX11(eAttributeType type, U32 count, const std::string& semantic_name, U32 semantic_idx, eInputSlotType slot_type, U32 input_slot, U32 step_rate)
    {
        VertexLayoutCommon* layout = (VertexLayoutCommon*)this;

        VertexAttributeCommon attribute;
        attribute.format_type  = type;
        attribute.format_count = count;
        attribute.sem_name     = semantic_name;
        attribute.sem_idx      = semantic_idx;
        attribute.slot_type    = slot_type;
        attribute.input_slot   = input_slot;
        attribute.step_rate    = step_rate;

        layout->m_attributes.push_back(attribute);
        layout->m_stride += VertexAttributeCommon::SizeOf(type) * count;
    }

    VertexLayoutCommon::VertexLayoutCommon()
    {
    }

    VertexLayoutCommon::~VertexLayoutCommon()
    {
    }

    void VertexLayoutCommon::Push(const VertexAttributeCommon& attribute)
    {
        m_stride += VertexAttributeCommon::SizeOf(attribute.format_type) * attribute.format_count;
        m_attributes.push_back(attribute);
    }

    void VertexLayoutCommon::Clear()
    {
        m_attributes.clear();
        m_stride = 0;
    }

    U32 VertexLayoutCommon::GetStride() const
    {
        return m_stride;
    }

    std::vector<VertexAttributeCommon>& VertexLayoutCommon::GetAttributes()
    {
        return m_attributes;
    }

    const std::vector<VertexAttributeCommon>& VertexLayoutCommon::GetAttributes() const
    {
        return m_attributes;
    }
}
