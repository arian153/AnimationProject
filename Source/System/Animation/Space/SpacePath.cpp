#include "SpacePath.hpp"

#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../Math/Curve/CurveGenerator.hpp"

namespace CS460
{
    SpacePath::SpacePath()
    {
    }

    SpacePath::~SpacePath()
    {
    }

    void SpacePath::CreateCurve()
    {
        if (control_points.empty())
            return;

        //control points size
        size_t size = control_points.size();

        if (size < 2)
            return;

        size_t bc_size = size - 1;

        segments.resize(bc_size);

        //find ai, bi for bezier curve
        //insert control point
        //a_i = p_i +(p_i+1 - p_i-1)/2
        //b_i = p i -(p_i+1 - p_i-1)/2
        //cubic bezier curve[i] = <p_i, a_i, b_i, p_i+1>

        Vector3 p_p  = Vector3();
        Vector3 p_c  = control_points[0];
        Vector3 p_n  = control_points[1];
        Vector3 p_nn = size == 2 ? Vector3() : control_points[2];

        Vector3 a_i = p_c + 0.5f * (p_n - p_p);
        Vector3 b_n = p_n - 0.5f * (p_nn - p_c);

        segments[0].p0 = control_points[0];
        segments[0].p1 = a_i;
        segments[0].p2 = b_n;
        segments[0].p3 = control_points[1];

        for (size_t i = 1; i < size - 2; ++i)
        {
            p_p  = control_points[i - 1];
            p_c  = control_points[i];
            p_n  = control_points[i + 1];
            p_nn = control_points[i + 2];

            a_i = p_c + 0.5f * (p_n - p_p);
            b_n = p_n - 0.5f * (p_nn - p_c);

            segments[i].p0 = control_points[i];
            segments[i].p1 = a_i;
            segments[i].p2 = b_n;
            segments[i].p3 = control_points[i + 1];
        }

        size_t i = size - 2;
        p_p      = size == 2 ? Vector3() : control_points[size - 3];
        p_c      = control_points[size - 2];
        p_n      = control_points[size - 1];
        p_nn     = Vector3();

        a_i = p_c + 0.5f * (p_n - p_p);
        b_n = p_n - 0.5f * (p_nn - p_c);

        segments[i].p0 = control_points[i];
        segments[i].p1 = a_i;
        segments[i].p2 = b_n;
        segments[i].p3 = control_points[i + 1];
    }

    void SpacePath::Update()
    {
        if (!b_update)
        {
            b_was_update = false;
            return;
        }

        CreateCurve();
        GenerateCurve();

        b_update     = false;
        b_was_update = true;
    }

    void SpacePath::BuildTable()
    {
    }

    Real SpacePath::InverseArcLength(Real s)
    {
        return s;
    }

    Real SpacePath::ArcLength(Real u)
    {
        return u;
    }

    void SpacePath::Draw(PrimitiveRenderer* renderer) const
    {
        if (!interpolated_curve.points.empty())
        {
            renderer->DrawSubMeshCurveLine(interpolated_curve, b_was_update, Color(1, 0, 0));
        }
    }

    void SpacePath::AddControlPoint(const Vector3& control_point)
    {
        control_points.push_back(control_point);
        b_update = true;
    }

    void SpacePath::SetSampleCount(size_t count)
    {
        sample_count = count;
        sample_step  = static_cast<Real>(1.0 / (double)count);
    }

    void SpacePath::GenerateCurve()
    {
        if (segments.empty())
            return;

        size_t segment_count = segments.size();
        size_t count         = sample_count / segment_count;
        sample_step          = static_cast<Real>(1.0 / (double)count);
        sample_count         = count * segment_count;
        //re-evaluate sample_step, sample_count.

        interpolated_curve.points.resize(sample_count + 1);
        for (size_t i = 0; i < segment_count; ++i)
        {
            Real u = 0.0f;
            segments[i].interpolated_points.clear();
            segments[i].interpolated_points.resize(count);
            for (size_t j = 0; j < count; ++j)
            {
                Vector3 point = segments[i].Interpolate(u);
                u += sample_step;

                segments[i].interpolated_points[j]       = point;
                interpolated_curve.points[i * count + j] = point;
            }
        }

        //set last point
        interpolated_curve.points.back() = segments.back().p3;
    }
}
