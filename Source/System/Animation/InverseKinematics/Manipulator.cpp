#include "Manipulator.hpp"

#include "Link.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../Skeleton/Bone.hpp"

namespace CS460
{
    Manipulator::Manipulator()
    {
        m_drawing_sphere.radius = 0.5f;
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
        //do jacobian
        if (dest_position.DistanceSquaredTo(curr_position) > error_threshold && converge_frame > 0)
        {
            //evaluate Jacobian matrix
            std::vector<Vector2> jacobian;
            EvaluateJacobian(jacobian);
            //calculate pseudo inverse Jacobian
            std::vector<Vector2> inverse;
            CalculatePseudoInverse(jacobian, inverse);

            Vector2 curr_velocity = (dest_position - curr_position) / (Real)converge_frame;
            curr_position         = ApplyAngleStep(inverse, curr_velocity, dt);
            converge_frame--;
        }
    }

    void Manipulator::UpdateAngles(const std::vector<Real>& angles)
    {
        size_t size = forward_links.size();
        for (size_t i = 0; i < size; ++i)
        {
            forward_links[i]->SetAngle(angles[i]);
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
                renderer->DrawPrimitiveInstancing(m_drawing_sphere, m_drawing_sphere.orientation, link->m_origin, eRenderingMode::Face, m_color);
            }

            forward_links.front()->Draw(renderer, m_color);
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

    Vector2 Manipulator::ApplyAngleStep(const std::vector<Vector2>& inverse, const Vector2& velocity, Real dt)
    {
        size_t size = inverse.size();
        //angle_steps = inverse jacobian * velocity;
        //link_angles = link_angles + angle_steps * dt;
        for (size_t i = 0; i < size; ++i)
        {
            //angle_steps[i] = inverse jacobian[i] * velocity;
            //link_angles[i] = link_angles[i] + angle_steps[i] * dt;
            forward_links[i]->m_angle += (inverse[i].x * velocity.x + inverse[i].y + velocity.y) / dt;
            forward_links[i]->UpdateAngle(false);
        }

        //curr_position = ForwardKinematics();
        return ForwardKinematics();
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
}
