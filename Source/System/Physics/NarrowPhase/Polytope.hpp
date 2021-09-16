#pragma once
#include "SupportPoint.hpp"
#include <vector>

namespace CS460
{
    class Simplex;

    class PolytopeEdge
    {
    public:
        PolytopeEdge();
        PolytopeEdge(size_t a, size_t b);
        PolytopeEdge(const SupportPoint& point_a, const SupportPoint& point_b);
        ~PolytopeEdge();

    public:
        size_t a, b;
    };

    class Polytope;

    class PolytopeFace
    {
    public:
        PolytopeFace(size_t a, size_t b, size_t c, const Vector3& normal, Real distance);
        PolytopeFace(size_t a, size_t b, size_t c, const Simplex& simplex);
        PolytopeFace(size_t a, size_t b, size_t c, const Polytope& polytope);
        PolytopeFace(const SupportPoint& point_a, const SupportPoint& point_b, const SupportPoint& point_c);
        ~PolytopeFace();

        size_t& operator[](size_t index);
        size_t  operator[](size_t index) const;

        PolytopeFace& operator=(const PolytopeFace& rhs);

        bool operator==(const PolytopeFace& rhs) const;

        size_t& Index(size_t index);
        size_t  Index(size_t index) const;

        Real TestFace(const Vector3& point) const;
        void BarycentricCoordinates(const Vector3& point, Real& u, Real& v, Real& w, Polytope* polytope) const;

    public:
        size_t  a, b, c;
        Vector3 normal;
        Real    distance;
    };

    class Polytope
    {
    public:
        explicit Polytope(const Simplex& simplex);
        ~Polytope();

        PolytopeFace PickClosestFace();
        Real         PointFaceTest(const Vector3& point, const Vector3& normal, const Vector3& v0);
        void         Push(SupportPoint& vertex);
        void         PushFromSimplex(const SupportPoint& vertex);
        void         Expand(const SupportPoint& vertex);
        bool         IsFaceSeen(const PolytopeFace& face, const SupportPoint& vertex);
        void         AddEdge(size_t a, size_t b);

    private:
        friend class NarrowPhase;
        friend class PolytopeFace;
    private:
        std::vector<SupportPoint> vertices;
        std::vector<PolytopeEdge> edges;
        std::vector<PolytopeFace> faces;
    };
}
