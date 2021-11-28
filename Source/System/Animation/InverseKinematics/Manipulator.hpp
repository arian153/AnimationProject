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
        void PushChild(Real length, Real angle);
        void PushParent(const Vector3& origin);
        void SetUpData();
        void Shutdown();

        void Draw(PrimitiveRenderer* renderer) const;

        void EvaluateJacobian(std::vector<Vector2>& jacobian);
        void CalculatePseudoInverse(const std::vector<Vector2>& jacobian, std::vector<Vector2>& inverse);
        void ApplyAngleStep(const std::vector<Vector2>& inverse, const Vector2& velocity, Real dt);
        void ApplyConstraintsAngleStep(const std::vector<Vector2>& inverse, const std::vector<Vector2>& jacobian, const Vector2& velocity, Real dt);

        Vector2 ForwardKinematics();

        Vector3 RootOrigin() const;
        Vector3 RootLink() const;
        Vector3 EndEffectorOrigin() const;

        bool IsReachable(const Vector3& position) const;

        void SetRootPosition(const Vector3& new_pos);

    public:
        std::vector<Link*> inverse_links;
        std::vector<Link*> forward_links;
        std::vector<Real>  constraints;

        Vector2 dest_position;
        Vector3 last_velocity;

        Real error_threshold   = 0.1f;
        int  converge_frame    = 0;
        bool apply_constraints = true;

        Real max_length = 1.0f;

        Sphere m_drawing_sphere;
        Color  m_color = Color(0.9f, 0.9f, 0.25f);
    };
}
