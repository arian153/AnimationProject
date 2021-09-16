#pragma once
#include "SupportPoint.hpp"

namespace CS460
{
    class ColliderPrimitive;

    class Simplex
    {
    public:
        Simplex();
        ~Simplex();
        SupportPoint& operator[](size_t index);
        SupportPoint  operator[](size_t index) const;

        bool DoSimplexPoint(Vector3& dir);
        bool DoSimplexLine(Vector3& dir);
        bool DoSimplexTriangle(Vector3& dir);
        bool DoSimplexTetrahedron(Vector3& dir);

        bool DoSimplex(Vector3& dir);
        bool IsValid();

        bool DoSimplex2DPoint(Vector3& dir);
        bool DoSimplex2DLine(Vector3& dir);
        bool DoSimplex2DTriangle(Vector3& dir);
        bool DoSimplex2D(Vector3& dir);

        void Set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c, const SupportPoint& d);
        void Set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c);
        void Set(const SupportPoint& a, const SupportPoint& b);
        void Set(const SupportPoint& a);

    private:
        friend class NarrowPhase;
        friend class Polytope;
        friend class IntersectionUtility;

    private:
        SupportPoint simplex_vertex_a; //point
        SupportPoint simplex_vertex_b; //line_segment
        SupportPoint simplex_vertex_c; //triangle
        SupportPoint simplex_vertex_d; //tetrahedron

        int count = 0;
    };

}
