#pragma once
#include "../Algebra/Vector3.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../../Physics/Utility/RayCastUtility.hpp"

namespace CS460
{
    class PrimitiveRenderer;
    enum class eRenderingMode;
    class Color;

    enum class ePrimitiveType
    {
        Circle
      , Ellipse
      , Polygon
      , Rectangle
      , Triangle
      , Box
      , Capsule
      , Cone
      , Cylinder
      , Ellipsoid
      , Polyhedron
      , Sphere
      , Tetrahedron
      , Invalid
    };

    class Primitive
    {
    public:
        Primitive();
        explicit Primitive(const Vector3& position, const Quaternion& orientation);
        virtual ~Primitive();

        virtual void CastRay(PrimitiveRayCastResult& result, Real max_distance = -1.0f) final;
        virtual void IntersectRay(PrimitiveRayIntersectionResult& result, Real max_distance = 1.0f) final;

        virtual Vector3       LocalToWorldPoint(const Vector3& local_point) const final;
        virtual Vector3       WorldToLocalPoint(const Vector3& world_point) const final;
        virtual Vector3       LocalToWorldVector(const Vector3& local_vector) const final;
        virtual Vector3       WorldToLocalVector(const Vector3& world_vector) const final;
        virtual ePrimitiveType Type() const final;

        void AddPosition(const Vector3& delta_pos);

    public:
        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        //Primitive Data
        virtual void SetUnit() = 0;

        //Minkowski Support - gjk, epa
        virtual Vector3 Support(const Vector3& direction) = 0;

        //Ray - Primitive Intersection
        virtual bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const = 0;
        virtual Vector3 GetNormal(const Vector3& local_point_on_primitive) = 0;

        //Draw
        virtual void DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const = 0;


    public:
        Vector3    position;
        Quaternion orientation;

    protected:
        ePrimitiveType type = ePrimitiveType::Invalid;
    };
}
