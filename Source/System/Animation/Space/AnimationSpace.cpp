#include "AnimationSpace.hpp"

#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../Math/Primitive/Others/Ray.hpp"

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

        m_moving_sphere.radius = 0.33f;
    }

    void AnimationSpace::Update([[maybe_unused]] Real dt)
    {
        m_platform_box.position    = m_box_transform.position;
        m_platform_box.orientation = m_box_transform.rotation;
        m_test_path.UpdateStatus();

        m_test_s += (m_test_speed / m_test_path.max_length) * dt;
        if (m_test_s >= 1.0f)
            m_test_s = 0.0f;
    }

    void AnimationSpace::Shutdown()
    {
    }

    void AnimationSpace::Draw(PrimitiveRenderer* renderer) const
    {
        renderer->DrawPrimitive(m_platform_box, eRenderingMode::Line, Color(0.2f, 0.2f, 0.2f));
        m_test_path.Draw(renderer);
        for (auto& points : control_points)
        {
            renderer->DrawPrimitiveInstancing(m_drawing_sphere, m_drawing_sphere.orientation, points, eRenderingMode::Face, Color(1, 0, 0));
        }

        Vector3 pos = m_test_path.SpaceCurve(m_test_path.InvArcLength(m_test_s));
        renderer->DrawPrimitiveInstancing(m_moving_sphere, m_moving_sphere.orientation, pos, eRenderingMode::Face, Color(0, 0, 1));
    }

    bool AnimationSpace::PickBox(const Ray& picking_ray, Real& min_t, Real& max_t)
    {
        Ray local_ray = picking_ray.ToLocal(m_box_transform);
        if (m_platform_box.TestRayIntersection(local_ray, min_t, max_t))
        {
            Real curr_s = m_test_s * m_test_path.max_length;
            control_points.push_back(picking_ray.ParamToPoint(min_t));
            m_test_path.AddControlPoint(picking_ray.ParamToPoint(min_t));
            m_test_s = Math::IsZero(m_test_path.max_length) ? 0.0f : curr_s / m_test_path.max_length;
            return true;
        }
        return false;
    }
}
