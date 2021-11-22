#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../../Math/Primitive/Others/Ray.hpp"
#include "../../Graphics/DataType/Color.hpp"

namespace CS460
{
    enum class eRayTestOption
    {
        Cast,
        Intersect,
        Trace
    };

    class RayTest
    {
    public:
        RayTest();
        RayTest(const Ray& r, eRayTestOption o, Real m, size_t rc, const Color& c);
        ~RayTest();

    public:
        Ray            ray;
        eRayTestOption option        = eRayTestOption::Cast;
        Real           max_distance  = -1.0f;
        size_t         reflect_count = 0;
        Color          color;
    };
}
