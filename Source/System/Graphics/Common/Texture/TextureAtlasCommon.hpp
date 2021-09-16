#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeTextureAtlasAPI
#include "../../../Math/Algebra/Vector4.hpp"
#include "../../../Math/Algebra/Vector2.hpp"
#include <vector>

namespace CS460
{
    class TextureAtlasCommon : public TextureAtlasAPI
    {
    public:
        TextureAtlasCommon();
        ~TextureAtlasCommon();

        Vector4 GetUVRect(size_t index);

        Vector2 ConvertUV(const Vector2& uv, size_t index);

    private:
        std::vector<Vector4> m_fragments;
    };
}
