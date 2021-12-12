#include "Simplex.hpp"

namespace CS460
{
    Simplex::Simplex()
    {
    }

    Simplex::~Simplex()
    {
    }

    SupportPoint& Simplex::operator[](size_t index)
    {
        switch (index)
        {
        case 0:
            return simplex_vertex_d;
        case 1:
            return simplex_vertex_c;
        case 2:
            return simplex_vertex_b;
        case 3:
            return simplex_vertex_a;
        default:
            return simplex_vertex_a;
        }
    }

    SupportPoint Simplex::operator[](size_t index) const
    {
        switch (index)
        {
        case 0:
            return simplex_vertex_d;
        case 1:
            return simplex_vertex_c;
        case 2:
            return simplex_vertex_b;
        case 3:
            return simplex_vertex_a;
        default:
            return simplex_vertex_a;
        }
    }

    bool Simplex::DoSimplexPoint(Vector3& dir)
    {
        dir              = -simplex_vertex_a.global;
        simplex_vertex_b = simplex_vertex_a;
        count            = 1;
        return false;
    }

    bool Simplex::DoSimplexLine(Vector3& dir)
    {
        Vector3 ab       = simplex_vertex_b.global - simplex_vertex_a.global;
        Vector3 ao       = -simplex_vertex_a.global;
        dir              = ab.CrossProductTwice(ao);
        simplex_vertex_c = simplex_vertex_b;
        simplex_vertex_b = simplex_vertex_a;
        count            = 2;
        return false;
    }

    bool Simplex::DoSimplexTriangle(Vector3& dir)
    {
        Vector3 ao  = -simplex_vertex_a.global;
        Vector3 ab  = simplex_vertex_b.global - simplex_vertex_a.global;
        Vector3 ac  = simplex_vertex_c.global - simplex_vertex_a.global;
        Vector3 abc = ab.CrossProduct(ac);
        //point is can't be behind/in the direction of B,C or BC
        Vector3 ab_abc = ab.CrossProduct(abc);
        // is the origin away from ab edge? in the same plane
        //if a0 is in that direction than
        if (ab_abc.DotProduct(ao) > 0.0f)
        {
            //change points
            simplex_vertex_c = simplex_vertex_a;
            //dir is not ab_abc because it's not point towards the origin
            dir = ab.CrossProductTwice(ao);
            //direction change; can't build tetrahedron
            return false;
        }
        Vector3 abc_ac = abc.CrossProduct(ac);
        // is the origin away from ac edge? or it is in abc?
        //if a0 is in that direction than
        if (abc_ac.DotProduct(ao) > 0.0f)
        {
            //keep c the same
            simplex_vertex_b = simplex_vertex_a;
            //dir is not abc_ac because it's not point towards the origin
            dir = ac.CrossProductTwice(ao);
            //direction change; can't build tetrahedron
            return false;
        }
        //now can build tetrahedron; check if it's above or below
        if (abc.DotProduct(ao) > 0.0f)
        {
            //base of tetrahedron
            simplex_vertex_d = simplex_vertex_c;
            simplex_vertex_c = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;
            //new direction
            dir = abc;
        }
        else
        {
            //upside down tetrahedron
            simplex_vertex_d = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;
            dir              = -abc;
        }
        count = 3;
        return false;
    }

    bool Simplex::DoSimplexTetrahedron(Vector3& dir)
    {
        Vector3 ao = -simplex_vertex_a.global;
        Vector3 ab = simplex_vertex_b.global - simplex_vertex_a.global;
        Vector3 ac = simplex_vertex_c.global - simplex_vertex_a.global;
        Vector3 ad = simplex_vertex_d.global - simplex_vertex_a.global;
        dir        = ab.CrossProduct(ac);
        if (ao.DotProduct(dir) > 0.0f)
        {
            //in front of abc
            simplex_vertex_d = simplex_vertex_c;
            simplex_vertex_c = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;
        }
        else
        {
            dir = ad.CrossProduct(ab);
            if (ao.DotProduct(dir) > 0.0f)
            {
                //in front of adb
                simplex_vertex_c = simplex_vertex_d;
                simplex_vertex_d = simplex_vertex_b;
                simplex_vertex_b = simplex_vertex_a;
            }
            else
            {
                dir = ac.CrossProduct(ad);
                if (ao.DotProduct(dir) > 0.0f)
                {
                    //n front of acd
                    simplex_vertex_b = simplex_vertex_a;
                }
                else
                {
                    // origin is in the tetrahedron abcd
                    count = 4;
                    return true;
                }
            }
        }
        count = 3;
        return false;
    }

    bool Simplex::DoSimplex(Vector3& dir)
    {
        if (count == 0)
        {
            return DoSimplexPoint(dir);
        }
        if (count == 1)
        {
            return DoSimplexLine(dir);
        }
        if (count == 2)
        {
            return DoSimplexTriangle(dir);
        }
        if (count == 3)
        {
            return DoSimplexTetrahedron(dir);
        }
        return false;
    }

    bool Simplex::DoSimplex2D(Vector3& dir)
    {
        if (count == 0)
        {
            return DoSimplex2DPoint(dir);
        }
        if (count == 1)
        {
            return DoSimplex2DLine(dir);
        }
        if (count == 2)
        {
            return DoSimplex2DTriangle(dir);
        }

        return false;
    }

    bool Simplex::IsValid()
    {
        if (simplex_vertex_d.IsValid() == false)
        {
            count = 0;
            return false;
        }
        if (simplex_vertex_c.IsValid() == false)
        {
            count = 1;
            return false;
        }
        if (simplex_vertex_b.IsValid() == false)
        {
            count = 2;
            return false;
        }
        if (simplex_vertex_a.IsValid() == false)
        {
            count = 3;
            return false;
        }
        return true;
    }

    bool Simplex::DoSimplex2DPoint(Vector3& dir)
    {
        dir              = -simplex_vertex_a.global;
        simplex_vertex_b = simplex_vertex_a;
        count            = 1;
        return false;
    }

    bool Simplex::DoSimplex2DLine(Vector3& dir)
    {
        Vector3 ab       = simplex_vertex_b.global - simplex_vertex_a.global;
        Vector3 ao       = -simplex_vertex_a.global;
        dir              = ab.CrossProductTwice(ao);
        simplex_vertex_c = simplex_vertex_b;
        simplex_vertex_b = simplex_vertex_a;
        count            = 2;
        return false;
    }

    bool Simplex::DoSimplex2DTriangle(Vector3& dir)
    {
        Vector3 ao  = -simplex_vertex_a.global;
        Vector3 ab  = simplex_vertex_b.global - simplex_vertex_a.global;
        Vector3 ac  = simplex_vertex_c.global - simplex_vertex_a.global;
        Vector3 abc = ab.CrossProduct(ac);
        //point is can't be behind/in the direction of B,C or BC
        Vector3 ab_abc = ab.CrossProduct(abc);
        // is the origin away from ab edge? in the same plane
        //if a0 is in that direction than
        if (ab_abc.DotProduct(ao) > 0.0f)
        {
            //change points
            simplex_vertex_c = simplex_vertex_a;
            //dir is not ab_abc because it's not point towards the origin
            dir = ab.CrossProductTwice(ao);
            //direction change; can't build tetrahedron
            return false;
        }
        Vector3 abc_ac = abc.CrossProduct(ac);
        // is the origin away from ac edge? or it is in abc?
        //if a0 is in that direction than
        if (abc_ac.DotProduct(ao) > 0.0f)
        {
            //keep c the same
            simplex_vertex_b = simplex_vertex_a;
            //dir is not abc_ac because it's not point towards the origin
            dir = ac.CrossProductTwice(ao);
            //direction change; can't build tetrahedron
            return false;
        }

        if (abc.DotProduct(ao) > 0.0f)
        {
            simplex_vertex_d = simplex_vertex_c;
            simplex_vertex_c = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;
            dir              = abc;
        }
        else
        {
            simplex_vertex_d = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;
            dir              = -abc;
        }
        count = 3;
        return true;
    }

    void Simplex::Set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c, const SupportPoint& d)
    {
        count                  = 4;
        this->simplex_vertex_a = a;
        this->simplex_vertex_b = b;
        this->simplex_vertex_c = c;
        this->simplex_vertex_d = d;
    }

    void Simplex::Set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c)
    {
        count                  = 3;
        this->simplex_vertex_a = a;
        this->simplex_vertex_b = b;
        this->simplex_vertex_c = c;
    }

    void Simplex::Set(const SupportPoint& a, const SupportPoint& b)
    {
        count                  = 2;
        this->simplex_vertex_a = a;
        this->simplex_vertex_b = b;
    }

    void Simplex::Set(const SupportPoint& a)
    {
        count                  = 1;
        this->simplex_vertex_a = a;
    }

    Vector3 Simplex::ClosestPointGlobal(const Vector3& point) const
    {
        if (count == 0)
            return simplex_vertex_a.global;

        if (count == 1)
            return ClosestPointLine(
                                    point,
                                    simplex_vertex_b.global,
                                    simplex_vertex_a.global);

        if (count == 2)
            return ClosestPointTriangle(
                                        point,
                                        simplex_vertex_c.global,
                                        simplex_vertex_b.global,
                                        simplex_vertex_a.global);

        if (count > 2)
            return ClosestPointTetrahedron(
                                           point,
                                           simplex_vertex_d.global,
                                           simplex_vertex_c.global,
                                           simplex_vertex_b.global,
                                           simplex_vertex_a.global);

        return point;
    }

    Vector3 Simplex::ClosestPointLocalA(const Vector3& point) const
    {
        if (count == 1)
            return simplex_vertex_b.local_a;

        if (count == 2)
            return ClosestPointLine(
                                    point,
                                    simplex_vertex_c.local_a,
                                    simplex_vertex_b.local_a);

        if (count == 3)
            return ClosestPointTriangle(
                                        point,
                                        simplex_vertex_d.local_a,
                                        simplex_vertex_c.local_a,
                                        simplex_vertex_b.local_a);

        if (count == 4)
            return ClosestPointTetrahedron(
                                           point,
                                           simplex_vertex_d.local_a,
                                           simplex_vertex_c.local_a,
                                           simplex_vertex_b.local_a,
                                           simplex_vertex_a.local_a);

        return point;
    }

    Vector3 Simplex::ClosestPointLocalB(const Vector3& point) const
    {
        if (count == 1)
            return simplex_vertex_b.local_b;

        if (count == 2)
            return ClosestPointLine(
                                    point,
                                    simplex_vertex_c.local_b,
                                    simplex_vertex_b.local_b);

        if (count == 3)
            return ClosestPointTriangle(
                                        point,
                                        simplex_vertex_d.local_b,
                                        simplex_vertex_c.local_b,
                                        simplex_vertex_b.local_b);

        if (count == 4)
            return ClosestPointTetrahedron(
                                           point,
                                           simplex_vertex_d.local_b,
                                           simplex_vertex_c.local_b,
                                           simplex_vertex_b.local_b,
                                           simplex_vertex_a.local_b);

        return point;
    }

    int Simplex::Count() const
    {
        return count;
    }

    Vector3 Simplex::ClosestPointLine(const Vector3& point, const Vector3& a, const Vector3& b)
    {
        Vector3 result;
        Vector3 ab = b - a;
        // Project point onto ab, but deferring divide by Dot(ab, ab)
        Real t = DotProduct(point - a, ab);
        if (t <= 0.0f)
        {
            // point projects outside the [a,b] interval, on the a side; clamp to a
            t      = 0.0f;
            result = a;
        }
        else
        {
            // Always nonnegative since denom = ||ab|| ∧ 2
            Real denom = DotProduct(ab, ab);
            if (t >= denom)
            {
                // point projects outside the [a,b] interval, on the b side; clamp to b
                t      = 1.0f;
                result = b;
            }
            else
            {
                // point projects inside the [a,b] interval; must do deferred divide now
                t      = t / denom;
                result = a + t * ab;
            }
        }

        return result;
    }

    Vector3 Simplex::ClosestPointTriangle(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c)
    {
        // Check if P in vertex region outside A
        Vector3 ab = b - a;
        Vector3 ac = c - a;
        Vector3 ap = point - a;
        Real    d1 = DotProduct(ab, ap);
        Real    d2 = DotProduct(ac, ap);
        if (d1 <= 0.0f && d2 <= 0.0f)
        {
            // barycentric coordinates (1,0,0)
            return a;
        }

        // Check if P in vertex region outside B
        Vector3 bp = point - b;
        Real    d3 = DotProduct(ab, bp);
        Real    d4 = DotProduct(ac, bp);
        if (d3 >= 0.0f && d4 <= d3)
        {
            // barycentric coordinates (0,1,0)
            return b;
        }

        // Check if P in edge region of AB, if so return projection of P onto AB
        Real vc = d1 * d4 - d3 * d2;
        if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
        {
            Real v = d1 / (d1 - d3);
            // barycentric coordinates (1-v,v,0)
            return a + v * ab;
        }
        // Check if P in vertex region outside C
        Vector3 cp = point - c;
        Real    d5 = DotProduct(ab, cp);
        Real    d6 = DotProduct(ac, cp);
        if (d6 >= 0.0f && d5 <= d6)
        {
            // barycentric coordinates (0,0,1)
            return c;
        }

        // Check if P in edge region of AC, if so return projection of P onto AC
        Real vb = d5 * d2 - d1 * d6;
        if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
        {
            Real w = d2 / (d2 - d6);
            // barycentric coordinates (1-w,0,w)
            return a + w * ac;
        }
        // Check if P in edge region of BC, if so return projection of P onto BC
        Real va = d3 * d6 - d5 * d4;
        if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
        {
            Real w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
            // barycentric coordinates (0,1-w,w)
            return b + w * (c - b);
        }
        // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
        Real denom = 1.0f / (va + vb + vc);
        Real v     = vb * denom;
        Real w     = vc * denom;
        // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
        return a + ab * v + ac * w;
    }

    Vector3 Simplex::ClosestPointTetrahedron(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d)
    {
        // Start out assuming point inside all half spaces, so closest to itself
        Vector3 closest_pt   = point;
        Real    best_sq_dist = Math::REAL_MAX;
        // If point outside face abc then compute closest point on abc
        if (PointOutsidePlane(point, a, b, c))
        {
            Vector3 q       = ClosestPointTriangle(point, a, b, c);
            Real    sq_dist = DotProduct(q - point, q - point);
            // Update best closest point if (squared) distance is less than current best
            if (sq_dist < best_sq_dist)
                best_sq_dist = sq_dist, closest_pt = q;
        }
        // Repeat test for face acd
        if (PointOutsidePlane(point, a, c, d))
        {
            Vector3 q       = ClosestPointTriangle(point, a, c, d);
            Real    sq_dist = DotProduct(q - point, q - point);
            if (sq_dist < best_sq_dist)
                best_sq_dist = sq_dist, closest_pt = q;
        }
        // Repeat test for face adb
        if (PointOutsidePlane(point, a, d, b))
        {
            Vector3 q       = ClosestPointTriangle(point, a, d, b);
            Real    sq_dist = DotProduct(q - point, q - point);
            if (sq_dist < best_sq_dist)
                best_sq_dist = sq_dist, closest_pt = q;
        }
        // Repeat test for face bdc
        if (PointOutsidePlane(point, b, d, c))
        {
            Vector3 q       = ClosestPointTriangle(point, b, d, c);
            Real    sq_dist = DotProduct(q - point, q - point);
            if (sq_dist < best_sq_dist)
                best_sq_dist = sq_dist, closest_pt = q;
        }
        return closest_pt;
    }

    bool Simplex::PointOutsidePlane(const Vector3& p, const Vector3& a, const Vector3& b, const Vector3& c)
    {
        // [AP AB AC] >= 0
        return DotProduct(p - a, CrossProduct(b - a, c - a)) >= 0.0f;
    }

    bool Simplex::PointOutsidePlane(const Vector3& p, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d)
    {
        // [AP AB AC]
        Real sign_p = DotProduct(p - a, CrossProduct(b - a, c - a));
        // [AD AB AC]
        Real sign_d = DotProduct(d - a, CrossProduct(b - a, c - a));
        // Points on opposite sides if expression signs are opposite
        return sign_p * sign_d < 0.0f;
    }
}
