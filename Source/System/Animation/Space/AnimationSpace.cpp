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
    }

    void AnimationSpace::Update([[maybe_unused]] Real dt)
    {
        m_platform_box.position    = m_box_transform.position;
        m_platform_box.orientation = m_box_transform.rotation;
        m_test_path.Update();
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
            renderer->DrawPrimitive(Sphere(points, Quaternion(), 0.1f), eRenderingMode::Face, Color(1, 0, 0));
        }
    }

    bool AnimationSpace::PickBox(const Ray& picking_ray, Real& min_t, Real& max_t)
    {
        Ray local_ray = picking_ray.ToLocal(m_box_transform);
        if (m_platform_box.TestRayIntersection(local_ray, min_t, max_t))
        {
            control_points.push_back(picking_ray.ParamToPoint(min_t));
            m_test_path.AddControlPoint(picking_ray.ParamToPoint(min_t));
            return true;
        }
        return false;
    }
}
