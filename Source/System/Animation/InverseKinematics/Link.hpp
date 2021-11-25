#pragma once
#include <vector>

#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Primitive/ConvexHull3D/Cone.hpp"

namespace CS460
{
    class Color;
    class PrimitiveRenderer;
    class Sphere;
    class Quaternion;

    class Link
    {
    public:
        Link();
        ~Link();

        explicit Link(const Vector3& origin);

        void SetUpData();
        void SetAngle(Real angle, bool b_recursive = false);
        void UpdateAngle(bool b_recursive);
        bool IsEndEffector() const;

        void Draw(PrimitiveRenderer* renderer, const Color& color) const;

        void AddTranslation(const Vector3& translation);

    private:
        friend class Manipulator;

    private:
        Link* m_parent       = nullptr;
        Link* m_child        = nullptr;

        Real m_angle  = 0.0f; //angle to child origin
        Real m_length = 0.0f;

        Vector3 m_origin; //origin for rendering
        Vector3 m_to_child;

        Vector3 m_basis         = Vector3(1, 0, 0); //basis for rotation initially x-axis 
        Vector3 m_rotation_axis = Vector3(0, 0, 1); //axis of rotation initially z-axis

        Cone m_drawing_cone;
    };
}
