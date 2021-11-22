#pragma once
#include "../../Math/Algebra/Vector3.hpp"

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
        void Forward(Real angle);
        void UpdateChild(const Quaternion& rotation);
        bool IsEndEffector() const;

        void Draw(PrimitiveRenderer* renderer,  const Color& color) const;

    private:
        friend class Manipulator;

    private:
        Link* m_parent = nullptr;
        Link* m_child  = nullptr;

        Vector3 m_origin;
        Real    m_angle = 0.0f; //angle to child origin
        Vector3 m_to_child;

        Vector3 m_basis      = Vector3(1, 0, 0);
        Vector3 m_polar_axis = Vector3(0, 0, 1); //
    };
}
