#pragma once
#include <vector>


#include "../../Graphics/DataType/Color.hpp"
#include "../../Math/Primitive/ConvexHull3D/Sphere.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class PrimitiveRenderer;
    class Vector3;
    class Link;
    class Bone;

    class Manipulator
    {
    public:
        Manipulator();
        ~Manipulator();

        void SetUpLinks(Bone* end_effector);
        void InverseKinematics();
        void ForwardKinematics(const std::vector<Real>& angles);
        void GetAngleVector(std::vector<Real>& angles);

        void PushChild(const Vector3& origin);
        void PushParent(const Vector3& origin);
        void SetUpData();
        void Shutdown();

        void Draw(PrimitiveRenderer* renderer) const;

    public:
        std::vector<Link*> inverse_links;
        std::vector<Link*> forward_links;

        Sphere m_drawing_sphere;
        Color m_color = Color(1, 1, 0.5f);
    };
}
