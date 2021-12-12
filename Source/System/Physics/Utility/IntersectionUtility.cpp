#include "IntersectionUtility.hpp"
#include "../NarrowPhase/Simplex.hpp"

namespace CS460
{
    IntersectionUtility::IntersectionUtility()
    {
    }

    IntersectionUtility::~IntersectionUtility()
    {
    }

    bool IntersectionUtility::HasIntersection(Primitive* a, Primitive* b) const
    {
        Simplex simplex;
        Vector3 direction = Vector3(0.0f, 1.0f, 0.0f);
        for (size_t i = 0; i < 50; ++i)
        {
            if (direction.IsValid() == false)
            {
                return false;
            }
            simplex.simplex_vertex_a = GenerateCSOSupport(a, b, direction);
            if (DotProduct(simplex.simplex_vertex_a.global, direction) < 0.0f)
            {
                return false;
            }
            if (simplex.DoSimplex(direction) == true)
            {
                return true;
            }
        }
        return false;
    }

  
    SupportPoint IntersectionUtility::GenerateCSOSupport(Primitive* a, Primitive* b, const Vector3& direction) const
    {
        // convert search direction to model space
        Vector3 local_dir_a = a->WorldToLocalVector(direction);
        Vector3 local_dir_b = b->WorldToLocalVector(-direction);
        // compute support points in model space
        Vector3 support_a = a->Support(local_dir_a);
        Vector3 support_b = b->Support(local_dir_b);
        return SupportPoint(a->LocalToWorldPoint(support_a) - b->LocalToWorldPoint(support_b), support_a, support_b);
    }
}
