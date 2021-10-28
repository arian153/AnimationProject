#include "AnimationSpace.hpp"

#include <string>

#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../Math/Primitive/Others/Ray.hpp"
#include "../Skeleton/Skeleton.hpp"

namespace CS460
{
    AnimationSpace::AnimationSpace()
    {
    }

    AnimationSpace::~AnimationSpace()
    {
    }

    void AnimationSpace::Initialize()
    {
        m_box_transform.position = Vector3(0.0f, -1.55f);
        m_platform_box.SetBox(100.0f, 1.0f, 100.0f);
        m_drawing_sphere.position.SetZero();
        m_drawing_sphere.orientation.SetIdentity();
        m_drawing_sphere.radius = 0.1f;
    }

    void AnimationSpace::Update(Real dt)
    {
        m_platform_box.position    = m_box_transform.position;
        m_platform_box.orientation = m_box_transform.rotation;
        m_elapsed_time += dt;

        if (!m_space_paths.empty() && m_space_paths.size() > (size_t)m_path_idx)
        {
            m_space_paths[m_path_idx].UpdateStatus();
        }
    }

    void AnimationSpace::Shutdown()
    {
    }

    void AnimationSpace::Draw(PrimitiveRenderer* renderer) const
    {
        renderer->DrawPrimitive(m_platform_box, eRenderingMode::Line, Color(0.2f, 0.2f, 0.2f));

        if (!m_space_paths.empty() && m_space_paths.size() > (size_t)m_path_idx)
        {
            m_space_paths[m_path_idx].Draw(renderer);
            for (auto& points : m_space_paths[m_path_idx].control_points)
            {
                renderer->DrawPrimitiveInstancing(m_drawing_sphere, m_drawing_sphere.orientation, points, eRenderingMode::Face, Color(1, 0, 0));
            }
        }
    }

    bool AnimationSpace::PickBox(const Ray& picking_ray, Real& min_t, Real& max_t)
    {
        if (m_space_paths.empty())
        {
            return false;
        }

        if (m_path_idx >= m_space_paths.size())
        {
            return false;
        }

        if (m_space_paths[m_path_idx].b_editable == false)
        {
            return false;
        }

        Ray local_ray = picking_ray.ToLocal(m_box_transform);
        if (m_platform_box.TestRayIntersection(local_ray, min_t, max_t))
        {
            if (m_skeleton)
            {
                Real curr_s = m_skeleton->GetSParam() * m_space_paths[m_path_idx].max_length;
                m_space_paths[m_path_idx].AddControlPoint(picking_ray.ParamToPoint(min_t));
                curr_s = Math::IsZero(m_space_paths[m_path_idx].max_length) ? 0.0f : curr_s / m_space_paths[m_path_idx].max_length;
                m_skeleton->SetSParam(curr_s);
            }
            else
            {
                m_space_paths[m_path_idx].AddControlPoint(picking_ray.ParamToPoint(min_t));
            }

            return true;
        }
        return false;
    }

    void AnimationSpace::AddControlPoint(const Vector3& point)
    {
        if (m_space_paths.empty())
        {
            return;
        }

        if (m_path_idx < m_space_paths.size())
        {
            m_space_paths[m_path_idx].AddControlPoint(point);
        }
    }

    int AnimationSpace::CreateNewPath(const std::string& name)
    {
        m_path_idx = (int)m_space_paths.size();
        m_space_paths.push_back(SpacePath());
        std::string new_name           = name == "" ? "Path " + std::to_string(m_path_idx) : name;
        m_space_paths[m_path_idx].name = new_name;
        path_names.push_back(new_name);
        return m_path_idx;
    }

    void AnimationSpace::SetIDX(int idx)
    {
        if ((int)m_space_paths.size() > idx)
        {
            m_path_idx = idx;
        }
    }

    void AnimationSpace::SetSkeleton(Skeleton* skeleton)
    {
        m_skeleton = skeleton;
    }

    void AnimationSpace::AddPath(int idx)
    {
        //m_skeleton->AddPath();
    }

    Vector3 AnimationSpace::GetPathPoint(Real s)
    {
        if (!m_space_paths.empty() && m_space_paths.size() > (size_t)m_path_idx)
            return m_space_paths[m_path_idx].SpaceCurve(m_space_paths[m_path_idx].InvArcLength(s));
        return Vector3();
    }

    std::string AnimationSpace::GetPathName(int idx)
    {
        if (!m_space_paths.empty() && m_space_paths.size() > (size_t)idx)
            return m_space_paths[idx].name;
        return "";
    }

    int AnimationSpace::GetPathIDX(const std::string& name)
    {
        int size = (int)m_space_paths.size();
        for (int i = 0; i < size; ++i)
        {
            if (m_space_paths[i].name == name)
                return i;
        }

        return -1;
    }

    void AnimationSpace::SetEditable(bool b_edit)
    {
        if (m_space_paths.empty())
        {
            return;
        }

        if (m_path_idx < m_space_paths.size())
        {
            m_space_paths[m_path_idx].b_editable = b_edit;
        }
    }
}
