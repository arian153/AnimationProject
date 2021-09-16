#pragma once

#include "SupportPoint.hpp"
#include <vector>

namespace CS460
{
    class Simplex;
    class Polytope2D;

   /* class Polytope2DEdge
    {
    public:
        Polytope2DEdge(size_t a, size_t b, const Vector3& normal, Real distance);
        Polytope2DEdge(size_t a, size_t b, const Simplex& simplex);
        Polytope2DEdge(size_t a, size_t b, const Polytope2D& polytope);
        Polytope2DEdge(const SupportPoint& point_a, const SupportPoint& point_b);
        ~Polytope2DEdge();

        size_t& operator[](size_t index);
        size_t  operator[](size_t index) const;

        Polytope2DEdge& operator=(const Polytope2DEdge& rhs);

        bool operator==(const Polytope2DEdge& rhs) const;

        size_t& Index(size_t index);
        size_t  Index(size_t index) const;



    public:
        size_t a, b;
        Vector3 normal;
        Real    distance;
    };

    class Polytope2D
    {
    public:
        Polytope2D();
        ~Polytope2D();

    private:

    };*/

   
}
