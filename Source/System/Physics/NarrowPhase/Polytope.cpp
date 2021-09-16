#include "Polytope.hpp"
#include "Simplex.hpp"
#include "../../Core/Utility/CoreDef.hpp"
#include "../../Core/Utility/CoreUtility.hpp"

namespace CS460
{
    PolytopeEdge::PolytopeEdge()
        : a(Core::U64_MAX), b(Core::U64_MAX)
    {
    }

    PolytopeEdge::PolytopeEdge(size_t a, size_t b)
        : a(a), b(b)
    {
    }

    PolytopeEdge::PolytopeEdge(const SupportPoint& point_a, const SupportPoint& point_b)
    {
        a = point_a.index;
        b = point_b.index;
    }

    PolytopeEdge::~PolytopeEdge()
    {
    }

    PolytopeFace::PolytopeFace(size_t a, size_t b, size_t c, const Vector3& normal, Real distance)
        : a(a), b(b), c(c), normal(normal), distance(distance)
    {
    }

    PolytopeFace::PolytopeFace(size_t a, size_t b, size_t c, const Simplex& simplex)
        : a(a), b(b), c(c)
    {
        normal = (simplex[b].global - simplex[a].global).CrossProduct(simplex[c].global - simplex[a].global);
        normal.SetNormalize();
        // compute distance
        distance = normal.DotProduct(simplex[a].global);
    }

    PolytopeFace::PolytopeFace(size_t a, size_t b, size_t c, const Polytope& polytope)
        : a(a), b(b), c(c)
    {
        normal = (polytope.vertices[b].global - polytope.vertices[a].global).CrossProduct(polytope.vertices[c].global - polytope.vertices[a].global);
        normal.SetNormalize();
        // compute distance
        distance = normal.DotProduct(polytope.vertices[a].global);
    }

    PolytopeFace::PolytopeFace(const SupportPoint& point_a, const SupportPoint& point_b, const SupportPoint& point_c)
    {
        a      = point_a.index;
        b      = point_b.index;
        c      = point_c.index;
        normal = (point_b.global - point_a.global).CrossProduct(point_c.global - point_a.global);
        normal.SetNormalize();
        distance = normal.DotProduct(point_a.global);
    }

    PolytopeFace::~PolytopeFace()
    {
    }

    size_t& PolytopeFace::operator[](size_t index)
    {
        switch (index)
        {
        case 0:
            return a;
        case 1:
            return b;
        case 2:
            return c;
        default:
            E5_ASSERT(false, "access invalid index");
            return a;
        }
    }

    size_t PolytopeFace::operator[](size_t index) const
    {
        switch (index)
        {
        case 0:
            return a;
        case 1:
            return b;
        case 2:
            return c;
        default:
            return Core::U64_MAX;
        }
    }

    PolytopeFace& PolytopeFace::operator=(const PolytopeFace& rhs)
    {
        if (this != &rhs)
        {
            this->a        = rhs.a;
            this->b        = rhs.b;
            this->c        = rhs.c;
            this->normal   = rhs.normal;
            this->distance = rhs.distance;
        }
        return *this;
    }

    bool PolytopeFace::operator==(const PolytopeFace& rhs) const
    {
        return (this->a == rhs.a && this->b == rhs.b && this->c == rhs.c);
    }

    size_t& PolytopeFace::Index(size_t index)
    {
        switch (index)
        {
        case 0:
            return a;
        case 1:
            return b;
        case 2:
            return c;
        default:
            E5_ASSERT(false, "access invalid index");
            return a;
        }
    }

    size_t PolytopeFace::Index(size_t index) const
    {
        switch (index)
        {
        case 0:
            return a;
        case 1:
            return b;
        case 2:
            return c;
        default:
            return Core::U64_MAX;
        }
    }

    Real PolytopeFace::TestFace(const Vector3& point) const
    {
        return normal.DotProduct(point);
    }

    void PolytopeFace::BarycentricCoordinates(const Vector3& point, Real& u, Real& v, Real& w, Polytope* polytope) const
    {
        Vector3 v0          = polytope->vertices[b].global - polytope->vertices[a].global;
        Vector3 v1          = polytope->vertices[c].global - polytope->vertices[a].global;
        Vector3 v2          = point - polytope->vertices[a].global;
        Real    d00         = v0.DotProduct(v0);
        Real    d01         = v0.DotProduct(v1);
        Real    d11         = v1.DotProduct(v1);
        Real    d20         = v2.DotProduct(v0);
        Real    d21         = v2.DotProduct(v1);
        Real    denominator = d00 * d11 - d01 * d01;
        v                   = (d11 * d20 - d01 * d21) / denominator;
        w                   = (d00 * d21 - d01 * d20) / denominator;
        u                   = 1.0f - v - w;
    }

    Polytope::Polytope(const Simplex& simplex)
    {
        PushFromSimplex(simplex.simplex_vertex_d);
        PushFromSimplex(simplex.simplex_vertex_c);
        PushFromSimplex(simplex.simplex_vertex_b);
        PushFromSimplex(simplex.simplex_vertex_a);
        faces.emplace_back(this->vertices[3], this->vertices[2], this->vertices[1]);
        faces.emplace_back(this->vertices[3], this->vertices[1], this->vertices[0]);
        faces.emplace_back(this->vertices[3], this->vertices[0], this->vertices[2]);
        faces.emplace_back(this->vertices[2], this->vertices[0], this->vertices[1]);
    }

    Polytope::~Polytope()
    {
    }

    PolytopeFace Polytope::PickClosestFace()
    {
        std::vector<PolytopeFace>::iterator closest_face_iterator = faces.end();
        Real                                closest_face_distance = Math::REAL_POSITIVE_MAX;
        //not initialized polytope return error polytope.
        if (faces.empty() == true)
        {
            E5_ASSERT(false, "Error : empty polytope face\n");
            return PolytopeFace(0, 0, 0, Vector3(), 0.0f);
        }
        for (auto it = faces.begin(); it != faces.end(); ++it)
        {
            if ((*it).distance < closest_face_distance)
            {
                closest_face_distance = (*it).distance;
                closest_face_iterator = it;
            }
        }
        if (closest_face_iterator == faces.end())
        {
            E5_ASSERT(false, "Error : fail to find polytope face\n");
            return PolytopeFace(0, 0, 0, Vector3(), 0.0f);
        }
        return *closest_face_iterator;
    }

    Real Polytope::PointFaceTest(const Vector3& point, const Vector3& normal, const Vector3& v0)
    {
        Vector3 unit = normal.Normalize();
        Real    d    = -unit.DotProduct(v0);
        return normal.DotProduct(point) + d;
    }

    void Polytope::Push(SupportPoint& vertex)
    {
        vertex.index = this->vertices.size();
        vertices.emplace_back(vertex);
    }

    void Polytope::PushFromSimplex(const SupportPoint& vertex)
    {
        auto index = this->vertices.size();
        vertices.emplace_back(vertex.global, vertex.local_a, vertex.local_b, index);
    }

    void Polytope::Expand(const SupportPoint& vertex)
    {
        this->edges.clear();
        for (auto it = faces.begin(); it != faces.end();)
        {
            if (IsFaceSeen(*it, vertex))
            {
                //remove face in faces;
                this->AddEdge(it->a, it->b);
                this->AddEdge(it->b, it->c);
                this->AddEdge(it->c, it->a);
                it = faces.erase(it);
            }
            else
            {
                ++it;
            }
        }
        //add a new face cover up hole on polytope.
        for (auto it = edges.begin(); it != edges.end(); ++it)
        {
            //add a new face
            faces.emplace_back(this->vertices[it->a], this->vertices[it->b], vertex);
        }
    }

    bool Polytope::IsFaceSeen(const PolytopeFace& face, const SupportPoint& vertex)
    {
        Vector3 v01    = vertices[face.b].global - vertices[face.a].global;
        Vector3 v02    = vertices[face.c].global - vertices[face.a].global;
        Vector3 normal = v01.CrossProduct(v02).Normalize();
        return (normal.DotProduct(vertex.global - vertices[face.a].global)) > 0.0f;
    }

    void Polytope::AddEdge(size_t a, size_t b)
    {
        for (auto it = edges.begin(); it != edges.end(); ++it)
        {
            if (it->a == b && it->b == a)
            {
                //opposite edge found, remove it and do not add new one
                edges.erase(it);
                return;
            }
        }
        edges.emplace_back(a, b);
    }
}
