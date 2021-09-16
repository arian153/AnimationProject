#include "OutsideSetFace.hpp"
#include "ColliderFace.hpp"

namespace CS460
{
    OutsideSetFace::OutsideSetFace(const ColliderFace& ref_face)
        : a(ref_face.a), b(ref_face.b), c(ref_face.c)
    {
    }

    OutsideSetFace::~OutsideSetFace()
    {
    }
}
