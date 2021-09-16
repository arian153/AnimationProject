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
        dir = -simplex_vertex_a.global;
        simplex_vertex_b = simplex_vertex_a;
        count = 1;
        return false;
    }

    bool Simplex::DoSimplex2DLine(Vector3& dir)
    {
        Vector3 ab = simplex_vertex_b.global - simplex_vertex_a.global;
        Vector3 ao = -simplex_vertex_a.global;
        dir = ab.CrossProductTwice(ao);
        simplex_vertex_c = simplex_vertex_b;
        simplex_vertex_b = simplex_vertex_a;
        count = 2;
        return false;
    }

    bool Simplex::DoSimplex2DTriangle(Vector3& dir)
    {
        Vector3 ao = -simplex_vertex_a.global;
        Vector3 ab = simplex_vertex_b.global - simplex_vertex_a.global;
        Vector3 ac = simplex_vertex_c.global - simplex_vertex_a.global;
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
            dir = abc;
        }
        else
        {
            simplex_vertex_d = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;
            dir = -abc;
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
}
