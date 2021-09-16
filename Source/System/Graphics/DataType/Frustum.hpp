#pragma once
#include "../../Math/Primitive/Others/Plane.hpp"

namespace CS460
{
    class Ray;
    class Matrix44;

    class Frustum
    {
    public:
        Frustum();
        ~Frustum();

        Frustum& operator=(const Frustum& rhs);

        void ConstructFrustum(Real depth, const Matrix44& projection, const Matrix44& view);

        bool IsContainPoint(Real x, Real y, Real z) const;
        bool IsContainPoint(const Vector3& position) const;
        bool IsContainCube(Real x, Real y, Real z, Real scale) const;
        bool IsContainSphere(Real x, Real y, Real z, Real radius) const;
        bool IsContainBox(Real x, Real y, Real z, Real scale_x, Real scale_y, Real scale_z) const;
        void IntersectRay(const Ray& ray, Vector3& start_point, Vector3& end_point) const;

        Vector3 GetVertex(size_t i);

        Plane  operator[](size_t i) const;
        Plane& operator[](size_t i);

    private:
        Plane m_planes[ 6 ];
    };
}
