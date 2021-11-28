#include "Manipulator.hpp"

#include "Link.hpp"
#include "../../../External/imgui/imgui.h"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../Skeleton/Bone.hpp"

namespace CS460
{
    Manipulator::Manipulator()
    {
        m_drawing_sphere.radius = 0.2f;
    }

    Manipulator::~Manipulator()
    {
    }

    void Manipulator::SetUpLinks(Bone* end_effector)
    {
        if (!end_effector->IsEndEffector())
        {
            return;
        }

        inverse_links.clear();
        std::vector<Bone*> to_parents;
        to_parents.push_back(end_effector);

        while (!to_parents.empty())
        {
            Bone* bone = to_parents.back();
            to_parents.pop_back();

            Link* link = new Link(bone->GetPosition());
            inverse_links.push_back(link);

            Bone* parent = bone->GetParent();
            if (parent != nullptr)
            {
                to_parents.push_back(parent);
            }
        }

        forward_links.clear();
        forward_links.reserve(inverse_links.size());
        Link* parent_link = nullptr;

        for (auto rit = inverse_links.rbegin(); rit != inverse_links.rend(); ++rit)
        {
            Link* link     = *rit;
            link->m_parent = parent_link;
            if (parent_link != nullptr)
            {
                parent_link->m_child = link;
            }

            forward_links.push_back(link);
            parent_link = link;
        }

        //set up recursively.
        forward_links.front()->SetUpData();
    }

    void Manipulator::InverseKinematics(Real dt)
    {
        Vector3 pos = RootOrigin();
        Vector2 curr_position; //= //ForwardKinematics();
        curr_position.x = forward_links.back()->m_origin.x;
        curr_position.y = forward_links.back()->m_origin.z;
        Vector2 for_pos = Vector2(pos.x, pos.z) + ForwardKinematics();

        //do jacobian
        if (dest_position.DistanceTo(curr_position) > error_threshold && converge_frame > 0)
        {
            //evaluate Jacobian matrix
            std::vector<Vector2> jacobian;
            EvaluateJacobian(jacobian);
            //calculate pseudo inverse Jacobian
            std::vector<Vector2> inverse;
            CalculatePseudoInverse(jacobian, inverse);

            Vector2 curr_velocity = (dest_position - curr_position) * 0.9f;
            last_velocity.x       = curr_velocity.x;
            last_velocity.z       = curr_velocity.y;

            if (apply_constraints)
            {
                ApplyConstraintsAngleStep(inverse, jacobian, curr_velocity, dt);
            }
            else
            {
                ApplyAngleStep(inverse, curr_velocity, dt);
            }

            converge_frame--;
        }
    }

    void Manipulator::UpdateAngles(const std::vector<Real>& angles)
    {
        size_t size = forward_links.size();
        for (size_t i = 0; i < size; ++i)
        {
            forward_links[i]->SetAngle(angles[i], false);
        }
    }

    void Manipulator::GetAngleVector(std::vector<Real>& angles)
    {
        angles.clear();
        size_t size = forward_links.size();
        angles.resize(size);

        for (size_t i = 0; i < size; ++i)
        {
            angles[i] = forward_links[i]->m_angle;
        }
    }

    void Manipulator::PushChild(const Vector3& origin)
    {
        Link* link = new Link(origin);
        if (forward_links.empty())
        {
            forward_links.push_back(link);
            inverse_links.push_back(link);
        }
        else
        {
            link->m_parent          = forward_links.back();
            link->m_parent->m_child = link;
            forward_links.push_back(link);
            inverse_links.insert(inverse_links.begin(), link);
        }
    }

    void Manipulator::PushChild(Real length, Real angle)
    {
        Link* link         = new Link(Vector3());
        link->m_angle      = angle;
        link->m_length     = length;
        link->m_to_child.x = length * cosf(angle);
        link->m_to_child.z = length * sinf(angle);

        if (forward_links.empty())
        {
            forward_links.push_back(link);
            inverse_links.push_back(link);
        }
        else
        {
            link->m_parent          = forward_links.back();
            link->m_parent->m_child = link;
            link->m_origin          = link->m_parent->m_origin + link->m_parent->m_to_child;

            forward_links.push_back(link);
            inverse_links.insert(inverse_links.begin(), link);
        }
    }

    void Manipulator::PushParent(const Vector3& origin)
    {
        Link* link = new Link(origin);
        if (inverse_links.empty())
        {
            inverse_links.push_back(link);
            forward_links.push_back(link);
        }
        else
        {
            link->m_parent          = inverse_links.back();
            link->m_parent->m_child = link;
            inverse_links.push_back(link);
            forward_links.insert(forward_links.begin(), link);
        }
    }

    void Manipulator::SetUpData()
    {
        forward_links.front()->SetUpData();
        max_length = 0.0f;
        for (auto& link : forward_links)
        {
            max_length += link->m_length;
        }
        max_length *= 0.9f;

        size_t size = forward_links.size();
        size = size > 0 ? size - 1 : 0;
        constraints = std::vector<Real>(size, 0.0f);
    }

    void Manipulator::Shutdown()
    {
        for (auto& link : forward_links)
        {
            delete link;
        }

        forward_links.clear();
        inverse_links.clear();
    }

    void Manipulator::Draw(PrimitiveRenderer* renderer) const
    {
        //Render skeleton structure
        if (!forward_links.empty())
        {
            size_t size = forward_links.size();

            for (size_t i = 0; i < size; ++i)
            {
                Link* link = forward_links[i];
                if (link->IsEndEffector())
                {
                    renderer->DrawPrimitiveInstancing(m_drawing_sphere, m_drawing_sphere.orientation, link->m_origin, eRenderingMode::Face, Color(0.9f, 0.69f, 0.17f));
                }
                else
                {
                    renderer->DrawPrimitiveInstancing(m_drawing_sphere, m_drawing_sphere.orientation, link->m_origin, eRenderingMode::Face, m_color);
                }

                link->Draw(renderer, m_color);
            }
        }
    }

    void Manipulator::EvaluateJacobian(std::vector<Vector2>& jacobian)
    {
        size_t size = forward_links.size() - 1;
        jacobian.resize(size);

        Real accumulated_angle = 0.0f;
        for (size_t i = 0; i < size; ++i)
        {
            //Calculate each segment for jacobian
            //EX : -Ln*sin(a0 + ... + an);, Ln*cos(a0 + ... + an);

            accumulated_angle += forward_links[i]->m_angle;
            jacobian[i].x = -forward_links[i]->m_length * sinf(accumulated_angle);
            jacobian[i].y = forward_links[i]->m_length * cosf(accumulated_angle);
        }

        for (size_t i = 0; i < size; ++i)
        {
            //Add full segment for jacobian
            //Li*cos(a0 + ... + ai) + Li+1 *cos(a0 + ... + ai + ai+1) + ... + L_last * cos(a0 + ... + a_last);
            for (size_t j = i + 1; j < size; ++j)
            {
                jacobian[i].x += jacobian[j].x;
                jacobian[i].y += jacobian[j].y;
            }
        }
    }

    void Manipulator::CalculatePseudoInverse(const std::vector<Vector2>& jacobian, std::vector<Vector2>& inverse)
    {
        Matrix22 pseudo_matrix(0.0f, 0.0f, 0.0f, 0.0f);
        //d[0] d[1]
        //d[2] d[3]
        size_t size = jacobian.size();
        inverse.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            pseudo_matrix.data[0] += jacobian[i].x * jacobian[i].x;
            pseudo_matrix.data[1] += jacobian[i].x * jacobian[i].y;
            pseudo_matrix.data[2] += jacobian[i].y * jacobian[i].x;
            pseudo_matrix.data[3] += jacobian[i].y * jacobian[i].y;
        }

        //inverse 2x2 matrix
        pseudo_matrix.SetInverse();
        //pseudo_matrix
        //[a, b] = [0, 1]
        //[c, d] = [2, 3]

        Real a = pseudo_matrix.data[0];
        Real b = pseudo_matrix.data[1];
        Real c = pseudo_matrix.data[2];
        Real d = pseudo_matrix.data[3];

        //calculate inverse matrix
        for (size_t i = 0; i < size; ++i)
        {
            inverse[i].x = jacobian[i].x * a + jacobian[i].y * c;
            inverse[i].y = jacobian[i].x * b + jacobian[i].y * d;
        }
    }

    void Manipulator::ApplyAngleStep(const std::vector<Vector2>& inverse, const Vector2& velocity, Real dt)
    {
        size_t size = inverse.size();
        //angle_steps = inverse jacobian * velocity;
        //link_angles = link_angles + angle_steps * dt;
        Real accumulated_angle = 0.0f;
        for (size_t i = 0; i < size; ++i)
        {
            //angle_steps[i] = inverse jacobian[i] * velocity;
            //link_angles[i] = link_angles[i] + angle_steps[i] * dt;
            forward_links[i]->m_angle += (inverse[i].x * velocity.x + inverse[i].y + velocity.y) * dt;
            //ImGui::Text("%f", forward_links[i]->m_angle);
            forward_links[i]->UpdateAngle(accumulated_angle);
            accumulated_angle += forward_links[i]->m_angle;
        }
    }

    void Manipulator::ApplyConstraintsAngleStep(const std::vector<Vector2>& inverse, const std::vector<Vector2>& jacobian, const Vector2& velocity, Real dt)
    {
        size_t size = inverse.size();

        Real    accumulated_angle = 0.0f;
        Vector2 jq_w;
        for (size_t i = 0; i < size; ++i)
        {
            //angle_steps = J(q)^+ * [p_dot + J(q)*w] - J(q)^+ * J(q) * w;
            //link_angles = link_angles + angle_steps * dt;
            jq_w.x = jacobian[i].x * constraints[i];
            jq_w.y = jacobian[i].y * constraints[i];

            Real a_term = inverse[i].x * (velocity.x + jq_w.x) + inverse[i].y * (velocity.y + jq_w.y);
            Real b_term = inverse[i].x * jq_w.x + inverse[i].y * jq_w.y;

            forward_links[i]->m_angle += (a_term - b_term) * dt;
            //ImGui::Text("%f", forward_links[i]->m_angle);
            forward_links[i]->UpdateAngle(accumulated_angle);
            accumulated_angle += forward_links[i]->m_angle;
        }
    }

    Vector2 Manipulator::ForwardKinematics()
    {
        //Calculate forward kinematics
        //x = L1 *cos(a1) + ... + Ln*cos(a1 + ... + an);
        //y = L1 *sin(a1) + ... + Ln*sin(a1 + ... + an);
        Vector2 result;
        size_t  size              = forward_links.size();
        Real    accumulated_angle = 0.0f;
        for (size_t i = 0; i < size; ++i)
        {
            accumulated_angle += forward_links[i]->m_angle;
            result.x += forward_links[i]->m_length * cosf(accumulated_angle);
            result.y += forward_links[i]->m_length * sinf(accumulated_angle);
        }
        return result;
    }

    Vector3 Manipulator::RootOrigin() const
    {
        return forward_links.front()->m_origin;
    }

    Vector3 Manipulator::RootLink() const
    {
        return forward_links.front()->m_to_child;
    }

    Vector3 Manipulator::EndEffectorOrigin() const
    {
        return inverse_links.front()->m_origin;
    }

    bool Manipulator::IsReachable(const Vector3& position) const
    {
        Vector3 origin = forward_links.front()->m_origin;
        return origin.DistanceSquaredTo(position) < max_length * max_length;
    }

    void Manipulator::SetRootPosition(const Vector3& new_pos)
    {
        forward_links.front()->AddTranslation(new_pos - forward_links.front()->m_origin);
    }
}
