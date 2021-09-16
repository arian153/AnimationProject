#include "Frustum.hpp"
#include "../../Math/Algebra/Matrix44.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../Utility/TextRenderer.hpp"

namespace CS460
{
    Frustum::Frustum()
    {
    }

    Frustum::~Frustum()
    {
    }

    Frustum& Frustum::operator=(const Frustum& rhs)
    {
        if (&rhs != this)
        {
            for (size_t i = 0; i < 6; ++i)
            {
                m_planes[i] = rhs.m_planes[i];
            }
        }
        return *this;
    }

    void Frustum::ConstructFrustum(Real depth, const Matrix44& projection, const Matrix44& view)
    {
        Matrix44 proj = projection;
        // Calculate the minimum Z distance in the frustum.
        Real z_min = -projection(3, 2) / projection(2, 2);
        Real r     = depth / (depth - z_min);
        proj(2, 2) = r;
        proj(3, 2) = -r * z_min;
        // Create the frustum matrix from the view matrix and updated projection matrix.
        Matrix44 view_proj = view * proj;
        // Calculate near plane of frustum.
        m_planes[0].a = view_proj(0, 3) + view_proj(0, 2);
        m_planes[0].b = view_proj(1, 3) + view_proj(1, 2);
        m_planes[0].c = view_proj(2, 3) + view_proj(2, 2);
        m_planes[0].d = view_proj(3, 3) + view_proj(3, 2);
        m_planes[0].SetNormalize();
        // Calculate far plane of frustum.
        m_planes[1].a = view_proj(0, 3) - view_proj(0, 2);
        m_planes[1].b = view_proj(1, 3) - view_proj(1, 2);
        m_planes[1].c = view_proj(2, 3) - view_proj(2, 2);
        m_planes[1].d = view_proj(3, 3) - view_proj(3, 2);
        m_planes[1].SetNormalize();
        // Calculate left plane of frustum.
        m_planes[2].a = view_proj(0, 3) + view_proj(0, 0);
        m_planes[2].b = view_proj(1, 3) + view_proj(1, 0);
        m_planes[2].c = view_proj(2, 3) + view_proj(2, 0);
        m_planes[2].d = view_proj(3, 3) + view_proj(3, 0);
        m_planes[2].SetNormalize();
        // Calculate right plane of frustum.
        m_planes[3].a = view_proj(0, 3) - view_proj(0, 0);
        m_planes[3].b = view_proj(1, 3) - view_proj(1, 0);
        m_planes[3].c = view_proj(2, 3) - view_proj(2, 0);
        m_planes[3].d = view_proj(3, 3) - view_proj(3, 0);
        m_planes[3].SetNormalize();
        // Calculate top plane of frustum.
        m_planes[4].a = view_proj(0, 3) - view_proj(0, 1);
        m_planes[4].b = view_proj(1, 3) - view_proj(1, 1);
        m_planes[4].c = view_proj(2, 3) - view_proj(2, 1);
        m_planes[4].d = view_proj(3, 3) - view_proj(3, 1);
        m_planes[4].SetNormalize();
        // Calculate bottom plane of frustum.
        m_planes[5].a = view_proj(0, 3) + view_proj(0, 1);
        m_planes[5].b = view_proj(1, 3) + view_proj(1, 1);
        m_planes[5].c = view_proj(2, 3) + view_proj(2, 1);
        m_planes[5].d = view_proj(3, 3) + view_proj(3, 1);
        m_planes[5].SetNormalize();
        /* E5_DRAW_TEXT_OUTPUT(
                             Vector2(520, 50), ColorDef::Pure::Blue,
                             "Near : ", m_planes[ 0 ].Normal(),
                             "\nFar : ", m_planes[ 1 ].Normal(),
                             "\nleft : ", m_planes[ 2 ].Normal(),
                             "\nright : ", m_planes[ 3 ].Normal(),
                             "\ntop : ", m_planes[ 4 ].Normal(),
                             "\nbottom : ", m_planes[ 5 ].Normal());*/
    }

    bool Frustum::IsContainPoint(Real x, Real y, Real z) const
    {
        // Check if the point is inside all six planes of the view frustum.
        Vector3 point(x, y, z);
        for (size_t i = 0; i < 6; ++i)
        {
            if (m_planes[i].PlaneTest(point) < 0.0f)
            {
                return false;
            }
        }
        return true;
    }

    bool Frustum::IsContainPoint(const Vector3& position) const
    {
        for (size_t i = 0; i < 6; ++i)
        {
            if (m_planes[i].PlaneTest(position) < 0.0f)
            {
                return false;
            }
        }
        return true;
    }

    bool Frustum::IsContainCube(Real x, Real y, Real z, Real scale) const
    {
        // Check if any one point of the cube is in the view frustum.
        Vector3 p0(x - scale, y - scale, z - scale);
        Vector3 p1(x + scale, y - scale, z - scale);
        Vector3 p2(x - scale, y + scale, z - scale);
        Vector3 p3(x + scale, y + scale, z - scale);
        Vector3 p4(x - scale, y - scale, z + scale);
        Vector3 p5(x + scale, y - scale, z + scale);
        Vector3 p6(x - scale, y + scale, z + scale);
        Vector3 p7(x + scale, y + scale, z + scale);
        for (size_t i = 0; i < 6; i++)
        {
            if (m_planes[i].PlaneTest(p0) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p1) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p2) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p3) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p4) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p5) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p6) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p7) >= 0.0f)
            {
                continue;
            }
            return false;
        }
        return true;
    }

    bool Frustum::IsContainSphere(Real x, Real y, Real z, Real radius) const
    {
        // Check if the radius of the sphere is inside the view frustum.
        Vector3 point(x, y, z);
        for (size_t i = 0; i < 6; i++)
        {
            if (m_planes[i].PlaneTest(point) < -radius)
            {
                return false;
            }
        }
        return true;
    }

    bool Frustum::IsContainBox(Real x, Real y, Real z, Real scale_x, Real scale_y, Real scale_z) const
    {
        // Check if any one point of the cube is in the view frustum.
        Vector3 p0(x - scale_x, y - scale_y, z - scale_z);
        Vector3 p1(x + scale_x, y - scale_y, z - scale_z);
        Vector3 p2(x - scale_x, y + scale_y, z - scale_z);
        Vector3 p3(x + scale_x, y + scale_y, z - scale_z);
        Vector3 p4(x - scale_x, y - scale_y, z + scale_z);
        Vector3 p5(x + scale_x, y - scale_y, z + scale_z);
        Vector3 p6(x - scale_x, y + scale_y, z + scale_z);
        Vector3 p7(x + scale_x, y + scale_y, z + scale_z);
        for (size_t i = 0; i < 6; i++)
        {
            if (m_planes[i].PlaneTest(p0) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p1) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p2) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p3) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p4) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p5) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p6) >= 0.0f)
            {
                continue;
            }
            if (m_planes[i].PlaneTest(p7) >= 0.0f)
            {
                continue;
            }
            return false;
        }
        return true;
    }

    void Frustum::IntersectRay(const Ray& ray, Vector3& start_point, Vector3& end_point) const
    {
        Real minimum_t = -1.0f;
        Real maximum_t = -1.0f;
        bool b_first   = true;
        for (size_t i = 0; i < 6; ++i)
        {
            Real t = -1.0f;
            if (m_planes[i].IntersectRay(ray, t))
            {
                if (b_first == true)
                {
                    minimum_t = t;
                    maximum_t = t;
                    b_first   = false;
                }
                else
                {
                    if (t > maximum_t)
                    {
                        maximum_t = t;
                    }
                    if (t < minimum_t)
                    {
                        minimum_t = t;
                    }
                }
            }
        }
        if (minimum_t < 0.0f && maximum_t < 0.0f)
        {
            return;
        }
        if (minimum_t <= 0.0f)
        {
            minimum_t = 0.0f;
        }
        start_point = ray.position + (minimum_t * ray.direction);
        end_point   = ray.position + (maximum_t * ray.direction);
    }

    Vector3 Frustum::GetVertex(size_t i)
    {
        //2-----------1
        //|           |
        //|           |
        //|           |
        //|           |
        //3-----------0
        //near < 4, far >=4
        Plane p1 = ((i % 4) + 1) % 4 < 2 ? m_planes[5] : m_planes[4];
        Plane p2 = (i % 4) < 2 ? m_planes[3] : m_planes[2];
        Plane p3 = i < 4 ? m_planes[0] : m_planes[1];
        if (Math::IsZero(p1.a))
        {
            Plane temp = p1;
            if (Math::IsZero(p2.a))
            {
                p1 = p3;
                p3 = temp;
            }
            else
            {
                p1 = p2;
                p2 = temp;
            }
        }
        //Do Gaussian Elimination for find x, y, z
        Real first_2_scale     = p2.a / p1.a;
        Real first_3_scale     = p3.a / p1.a;
        Real first_step_p2_y   = p2.b - (first_2_scale * p1.b);
        Real first_step_p2_z   = p2.c - (first_2_scale * p1.c);
        Real first_step_p2_d   = -(p2.d - (first_2_scale * p1.d));
        Real first_step_p3_y   = p3.b - (first_3_scale * p1.b);
        Real first_step_p3_z   = p3.c - (first_3_scale * p1.c);
        Real first_step_p3_d   = -(p3.d - (first_3_scale * p1.d));
        Real second_step_scale = first_step_p3_y / first_step_p2_y;
        Real second_step_p3_z  = first_step_p3_z - (second_step_scale * first_step_p2_z);
        Real second_step_p3_d  = first_step_p3_d - (second_step_scale * first_step_p2_d);
        Real z                 = second_step_p3_d / second_step_p3_z;
        Real y                 = (-first_step_p2_z * z + first_step_p2_d) / first_step_p2_y;
        Real x                 = -(p1.b * y + p1.c * z + p1.d) / p1.a;
        return Vector3(x, y, z);
    }

    Plane Frustum::operator[](size_t i) const
    {
        return m_planes[i];
    }

    Plane& Frustum::operator[](size_t i)
    {
        return m_planes[i];
    }
}
