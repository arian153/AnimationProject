#include "TextureAtlasDX11.hpp"
#include "../../Common/Texture/TextureAtlasCommon.hpp"

namespace CS460
{
    TextureAtlasDX11::TextureAtlasDX11()
    {
    }

    TextureAtlasDX11::~TextureAtlasDX11()
    {
    }

    TextureAtlasCommon::TextureAtlasCommon()
    {
    }

    TextureAtlasCommon::~TextureAtlasCommon()
    {
    }

    Vector4 TextureAtlasCommon::GetUVRect(size_t index)
    {
        return m_fragments.at(index);
    }

    Vector2 TextureAtlasCommon::ConvertUV(const Vector2& uv, size_t index)
    {
        Vector4 rect = m_fragments.at(index);
        return Vector2(rect.x + uv.x * (rect.z - rect.x), rect.y + uv.y * (rect.w - rect.y));
    }
}
