#include "TextureVertexDX11.hpp"
#include "../../Common/Vertex/TextureVertexCommon.hpp"
#include "../../../Math/Algebra/Vector2.hpp"
#include "../../../Math/Algebra/Vector3.hpp"
#include "../ConverterDX11.hpp"

namespace CS460
{
    TextureVertexDX11::TextureVertexDX11()
        : position(), uv()
    {
    }

    TextureVertexDX11::~TextureVertexDX11()
    {
    }

    TextureVertexCommon::TextureVertexCommon()
    {
    }

    TextureVertexCommon::TextureVertexCommon(Real px, Real py, Real pz, Real u, Real v)
    {
        position.x = px;
        position.y = py;
        position.z = pz;
        uv.x = u;
        uv.y = v;
    }

    TextureVertexCommon::TextureVertexCommon(const Vector3& p, const Vector2& _uv)
    {
        position = ConverterDX11::ToXMFloat3(p);
        uv = ConverterDX11::ToXMFloat2(_uv);
    }

    Vector3 TextureVertexCommon::GetPosition() const
    {
        return ConverterDX11::ToVector3(position);
    }

    Vector2 TextureVertexCommon::GetUV() const
    {
        return Vector2(uv.x, uv.y);
    }

    void TextureVertexCommon::SetPosition(const Vector3& p)
    {
        position = ConverterDX11::ToXMFloat3(p);
    }

    void TextureVertexCommon::SetUV(const Vector2& _uv)
    {
        uv = ConverterDX11::ToXMFloat2(_uv);
    }
}
