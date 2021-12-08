#include "TransformSpace.hpp"

#include "../../Core/Utility/CoreUtility.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    bool RayCastData::operator<(const RayCastData& rhs) const
    {
        return t > rhs.t;
    }

    RayCastData& RayCastData::operator=(const RayCastData& rhs)
    {
        if (this != &rhs)
        {
            transform = rhs.transform;
            hit       = rhs.hit;
            t         = rhs.t;
        }
        return *this;
    }

    TransformSpace::TransformSpace()
    {
    }

    TransformSpace::~TransformSpace()
    {
    }

    void TransformSpace::Initialize()
    {
        m_drawing_box.SetUnit();
    }

    void TransformSpace::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);

        //do update spatial data structure.
        //...
    }

    void TransformSpace::Shutdown()
    {
        // shutdown spatial data structure.

        m_transforms.clear();
    }

    void TransformSpace::Render(PrimitiveRenderer* primitive_renderer, const Color& color)
    {
        for (auto& transform : m_transforms)
        {
            primitive_renderer->DrawPrimitiveInstancing(m_drawing_box, transform->orientation, transform->position, transform->scale, eRenderingMode::Line, color);
        }
    }

    void TransformSpace::Add(Transform* transform)
    {
        m_transforms.push_back(transform);
    }

    void TransformSpace::Remove(Transform* transform)
    {
        if (!m_transforms.empty())
        {
            auto found = std::find(m_transforms.begin(), m_transforms.end(), transform);
            m_transforms.erase(found);
        }
    }

    void TransformSpace::CastRay(RayCastData& result, Real max_distance) const
    {
        Box  cast_box;
        Real min_t = -1.0f;
        Real max_t = -1.0f;
        for (auto& transform : m_transforms)
        {
            cast_box.SetBox(transform->scale.x, transform->scale.y, transform->scale.z);
            cast_box.position    = transform->position;
            cast_box.orientation = transform->orientation;

            if (cast_box.TestRayIntersection(result.ray, min_t, max_t))
            {
                if (result.hit && result.t < min_t)
                {
                    continue;
                }

                if (min_t > max_distance && max_distance >= 0.0f)
                {
                    continue;
                }

                result.t   = min_t;
                result.hit = true;
                //ray cast done
                result.intersection = result.ray.position + result.ray.direction * result.t;
            }
        }
    }
}
