#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include <vector>

namespace CS460
{
    class ColliderFace;

    class OutsideSetFace
    {
    public:
        explicit OutsideSetFace(const ColliderFace& ref_face);
        ~OutsideSetFace();

    public:
        size_t a = Core::U64_MAX;
        size_t b = Core::U64_MAX;
        size_t c = Core::U64_MAX;
        std::vector<Vector3> outside_vertices;
    };
}
