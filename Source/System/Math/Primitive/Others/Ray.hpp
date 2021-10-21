#pragma once
#include "../../Algebra/Vector3.hpp"
#include "../../Algebra/Quaternion.hpp"
#include "../../Structure/Vector3Pair.hpp"

namespace CS460
{
    class VQSTransform;
    class Line;
    class Ray
    {
    public:
        explicit Ray(const Vector3& position = Vector3(), const Vector3& direction = Math::Vector3::Y_AXIS);
        Ray(const Ray& rhs);
        ~Ray();
        Vector3 ParamToPoint(Real t) const;
        Real    PointToParam(const Vector3& point_on_ray) const;
        Ray     GetReflectedRay(const Vector3& normal, const Vector3& new_position) const;
        Vector3 SymmetricPoint(const Vector3& point) const;
        Vector3 ClosestPoint(const Vector3& point) const;
        Real    Distance(const Vector3& point, Real& t) const;
        Real    DistanceSquared(const Vector3& point, Real& t) const;
        Vector3Pair ClosestPoint(const Ray& ray) const;

        Ray ToLocal(const VQSTransform& transform) const;
        Ray ToWorld(const VQSTransform& transform) const;

        Ray&    operator=(const Ray& rhs);
        bool    operator==(const Ray& rhs) const;
        bool    operator!=(const Ray& rhs) const;

    public:
        friend Real Distance(const Ray& ray0, const Ray& ray1, Real& s, Real& t);
        friend Real Distance(const Ray& ray, const Vector3& point, Real& t);
        friend Real Distance(const Ray& ray, const Line& line, Real& s, Real& t);
        friend Real Distance(const Line& line, const Ray& ray, Real& s, Real& t);

        friend Real DistanceSquared(const Ray& ray0, const Ray& ray1, Real& s, Real& t);
        friend Real DistanceSquared(const Ray& ray, const Vector3& point, Real& t);
        friend Real DistanceSquared(const Ray& ray, const Line& line, Real& s, Real& t);
        friend Real DistanceSquared(const Line& line, const Ray& ray, Real& s, Real& t);

        friend Vector3Pair ClosestPoint(const Ray& ray0, const Ray& ray1);
        friend Vector3Pair ClosestPoint(const Ray& ray, const Line& line);

    public:
        Vector3 position;
        Vector3 direction;
    };
}
