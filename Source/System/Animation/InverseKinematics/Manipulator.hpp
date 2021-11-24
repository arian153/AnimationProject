#pragma once
#include <vector>

#include "../../Graphics/DataType/Color.hpp"
#include "../../Math/Primitive/ConvexHull3D/Sphere.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class PrimitiveRenderer;
    class Vector2;
    class Vector3;
    class Link;
    class Bone;

    class Manipulator
    {
    public:
        Manipulator();
        ~Manipulator();

        void SetUpLinks(Bone* end_effector);
        void InverseKinematics(Real dt);
        void UpdateAngles(const std::vector<Real>& angles);
        void GetAngleVector(std::vector<Real>& angles);

        void PushChild(const Vector3& origin);
        void PushParent(const Vector3& origin);
        void SetUpData();
        void Shutdown();

        void Draw(PrimitiveRenderer* renderer) const;

        void EvaluateJacobian(std::vector<Vector2>& jacobian);
        void CalculatePseudoInverse(const std::vector<Vector2>& jacobian, std::vector<Vector2>& inverse);

        Vector2 ApplyAngleStep(const std::vector<Vector2>& inverse, const Vector2& velocity, Real dt);
        Vector2 ForwardKinematics();

    public:
        std::vector<Link*> inverse_links;
        std::vector<Link*> forward_links;

        Vector2 dest_position;
        Vector2 curr_position;

        Real error_threshold = 0.1f;
        int  converge_frame  = 100;

        Sphere m_drawing_sphere;
        Color  m_color = Color(1, 1, 0.5f);
    };
}
