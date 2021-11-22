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

    void Manipulator::InverseKinematics()
    {
        //do jacobian
        Vector3 dest_position;
        Vector3 curr_position;

        Real error_threshold = 0.1f;
        int  converge_frame  = 100;

        if (dest_position.DistanceSquaredTo(curr_position) > error_threshold && converge_frame > 0)
        {
            //evaluate Jacobian matrix

            //calculate pseudo inverse Jacobian

            //Vector3 curr_velocity = (dest_position - curr_position)/converge_frame;

            //d_rot_params = inv_Jacobian * curr_velocity;

            //curr_rot_params = curr_rot_params + d_rot_params / dt;

            //curr_position = ForwardKinematics();
            converge_frame--;
        }
    }

    void Manipulator::ForwardKinematics(const std::vector<Real>& angles)
    {
        size_t size = forward_links.size();
        for (size_t i = 0; i < size; ++i)
        {
            forward_links[i]->Forward(angles[i]);
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
}
