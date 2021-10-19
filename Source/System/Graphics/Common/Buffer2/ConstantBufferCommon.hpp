#pragma once

#include <vector>

#include "../../GraphicsAPI.hpp"
#include IncludeConstantBufferAPI

#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Math/Structure/VQSTransform.hpp"
#include "../../DataType/BufferBindType.hpp"

namespace CS460
{
    struct MaterialBufferData;
    struct MultipleLightsBufferData;
    struct TextureBufferData;
    enum class eBindingStage;
    struct ColorBufferData;
    struct CameraBufferData;
    struct GammaBufferData;
    struct LightBufferData;
    struct MatrixBufferData;
    struct MatrixBufferDataInstancing;
    struct SkinnedBufferData;
    class Color;
    class RendererCommon;

    class ConstantBufferCommon : public ConstantBufferAPI
    {
    public:
        ConstantBufferCommon();
        ~ConstantBufferCommon();

        bool Init(RendererCommon* renderer, eBindingStage type, size_t buffer_size, U32 slot);
        void Update(void* data) const;
        void Update(const ColorBufferData& data) const;
        void Update(const CameraBufferData& data) const;
        void Update(const GammaBufferData& data) const;
        void Update(const LightBufferData& data) const;
        void Update(const MatrixBufferData& data) const;
        void Update(const MatrixBufferDataInstancing& data) const;
        void Update(const TextureBufferData& data) const;
        void Update(const MultipleLightsBufferData& data) const;
        void Update(const MaterialBufferData& data) const;
        void Update(const SkinnedBufferData& data) const;
        void Update(const std::vector<VQSTransform>& data) const;

        void* Map() const;
        void  UnMap() const;

        void Bind() const;
        void Shutdown();
    private:
        U32           m_slot  = 0;
        eBindingStage m_stage = eBindingStage::PixelShader;
    };
}
