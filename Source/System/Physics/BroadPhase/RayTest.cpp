#include "RayTest.hpp"

namespace CS460
{
    RayTest::RayTest()
    {
    }

    RayTest::RayTest(const Ray& r, eRayTestOption o, Real m, size_t rc, const Color& c)
        : ray(r), option(o), max_distance(m), reflect_count(rc), color(c)
    {
    }

    RayTest::~RayTest()
    {
    }
}
