#include "Color.hpp"
#include "../../Math/Utility/Utility.inl"

namespace CS460
{
    Color::Color(Real r, Real g, Real b, Real a)
        : r(r), g(g), b(b), a(a)
    {
    }

    /*Color::Color(U8 r, U8 g, U8 b, U8 a)
    {
        this->r = static_cast<Real>(r) / 255.0f;
        this->g = static_cast<Real>(g) / 255.0f;
        this->b = static_cast<Real>(b) / 255.0f;
        this->a = static_cast<Real>(a) / 255.0f;
    }*/

    Color::~Color()
    {
    }

    bool Color::operator==(const Color& rhs) const
    {
        return Math::IsEqual(r, rhs.r) &&
                Math::IsEqual(g, rhs.g) &&
                Math::IsEqual(b, rhs.b) &&
                Math::IsEqual(a, rhs.a);
    }

    Real Color::ToReal(U8 color)
    {
        return static_cast<Real>(color) / 255.0f;
    }

    Real Color::ToReal(int color)
    {
        return static_cast<Real>(color) / 255.0f;
    }

    U8 Color::ToByte(Real color)
    {
        return static_cast<U8>(color) * 255;
    }

    int Color::ToInt(Real color)
    {
        return static_cast<int>(color) * 255;
    }
}
