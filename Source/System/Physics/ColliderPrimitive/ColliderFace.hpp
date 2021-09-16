#pragma once
#include "../../Core/Utility/CoreDef.hpp"

namespace CS460
{
    class ColliderFace
    {
    public:
        ColliderFace();
        ColliderFace(size_t a, size_t b, size_t c);
        ~ColliderFace();

    public:
        size_t a = Core::U64_MAX;
        size_t b = Core::U64_MAX;
        size_t c = Core::U64_MAX;
    };
}
