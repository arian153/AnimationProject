#include "SpacePath.hpp"

#include <deque>

#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../Math/Curve/CurveGenerator.hpp"

namespace CS460
{
    ArcSegment::ArcSegment(Real a, Real b)
        : u_a(a), u_b(b)
    {
    }

    ArcData::ArcData(Real u, Real s)
        : arc_param(u), arc_length(s)
    {
    }

    SpacePath::SpacePath()
    {
    }

    SpacePath::~SpacePath()
    {
    }

    void SpacePath::UpdateStatus()
    {
        if (!b_update)
        {
            b_was_update = false;
            return;
        }

        b_update     = false;
        b_was_update = true;
    }

    void SpacePath::Draw(PrimitiveRenderer* renderer) const
    {
        if (!interpolated_curve.points.empty() && b_draw)
        {
            //replace curve line
            renderer->DrawSubMeshCurveLine(interpolated_curve, b_was_update, Color(1, 0, 0));
        }
    }

    void SpacePath::AddControlPoint(const Vector3& control_point)
    {
        control_points.push_back(control_point);
        SetUpSegments();
        GenerateRenderingCurve();
        AdaptiveApproach();
        b_update = true;
    }

    void SpacePath::SetRenderingCurveSampleCount(size_t count)
    {
        sample_count = count;
    }

    Vector3 SpacePath::SpaceCurve(Real u) const
    {
        if (segments.empty())
            return Vector3();
        //clamp u
        if (u <= 0.0f)
        {
            //result is first control point
            return segments.front().p0;
        }

        if (u >= 1.0f)
        {
            //result is last control point
            return segments.back().p3;
        }

        size_t segment_count = segments.size();
        for (size_t i = 0; i < segment_count; ++i)
        {
            Real u_min = (Real)i / (Real)segment_count;
            Real u_max = (Real)(i + 1) / (Real)segment_count;

            //check u is on correct segment
            if (u_min <= u && u <= u_max)
            {
                Real normalized_u = (u - u_min) / (u_max - u_min);
                return segments[i].Interpolate(normalized_u);
            }
        }

        //no found this is error
        return Vector3();
    }

    Real SpacePath::ArcLength(Real u) const
    {
        if (arc_length_table.empty())
            return -1.0f;

        if (arc_length_table.size() == 1)
            return arc_length_table[0].arc_length;

        size_t l = 0;
        size_t r = arc_length_table.size() - 1;
        while (l <= r)
        {
            size_t m = l + (r - l) / 2;

            // Check if u is equal at mid
            if (Math::IsEqual(arc_length_table[m].arc_param, u))
                return arc_length_table[m].arc_length;

            if (arc_length_table[m].arc_param < u)
            {
                // If u greater, ignore left half
                l = m + 1;
            }
            else
            {
                // If u is smaller, ignore right half
                r = m - 1;
            }
        }

        //not found fail to search 
        return -1.0f;
    }

    Real SpacePath::InvArcLength(Real s) const
    {
        if (arc_length_table.empty())
            return -1.0f;

        if (arc_length_table.size() == 1)
            return arc_length_table[0].arc_param;

        size_t l = 0;
        size_t r = arc_length_table.size() - 1;
        size_t m = l + (r - l) / 2; //default m
        while (l <= r)
        {
            m = l + (r - l) / 2;

            // Check if s is equal at mid
            if (Math::IsEqual(arc_length_table[m].arc_length, s))
                return arc_length_table[m].arc_param;

            if (arc_length_table[m].arc_length < s)
            {
                // If s greater, ignore left half
                l = m + 1;
            }
            else
            {
                // If s is smaller, ignore right half
                r = m - 1;
            }
        }

        //not found fail to search but normalize it
        if (arc_length_table[m].arc_length > s)
        {
            if (m > 0)
            {
                Real s_min = arc_length_table[m - 1].arc_length;
                Real s_max = arc_length_table[m].arc_length;
                Real u_min = arc_length_table[m - 1].arc_param;
                Real u_max = arc_length_table[m].arc_param;

                //Get delta u from rendering curve
                Real d_u = u_max - u_min;
                return u_min + d_u * ((s - s_min) / (s_max - s_min));
            }
        }
        else if (arc_length_table[m].arc_length < s)
        {
            if (m < arc_length_table.size() - 1)
            {
                Real s_min = arc_length_table[m].arc_length;
                Real s_max = arc_length_table[m + 1].arc_length;
                Real u_min = arc_length_table[m].arc_param;
                Real u_max = arc_length_table[m + 1].arc_param;

                //Get delta u from rendering curve
                Real d_u = u_max - u_min;
                return u_min + d_u * ((s - s_min) / (s_max - s_min));
            }
        }
        return -1.0f;
    }

    void SpacePath::LoadData(const std::vector<Vector3>& points)
    {
        control_points = points;
        SetUpSegments();
        GenerateRenderingCurve();
        AdaptiveApproach();
        b_update = true;
    }

    void SpacePath::SetUpSegments()
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

    void SpacePath::GenerateRenderingCurve()
    {
        if (segments.empty())
            return;

        size_t segment_count = segments.size();
        size_t count         = sample_count / segment_count;
        Real   sample_step   = static_cast<Real>(1.0 / (double)count);
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

    void SpacePath::AdaptiveApproach()
    {
        if (segments.empty())
            return;

        //reset table
        arc_length_table.clear();
        arc_length_table.push_back(ArcData(0.0f, 0.0f));
        std::deque<ArcSegment> segment_list;
        segment_list.push_back(ArcSegment(0.0f, 1.0f));

        while (!segment_list.empty())
        {
            ArcSegment segment = segment_list.front();
            segment_list.pop_front();

            Real    u_m = 0.5f * (segment.u_a + segment.u_b);
            Vector3 p_a = SpaceCurve(segment.u_a);
            Vector3 p_b = SpaceCurve(segment.u_b);
            Vector3 p_m = SpaceCurve(u_m);

            Real dist_a = p_a.DistanceTo(p_m); //A = |P(u_a) - P(u_m)|;
            Real dist_b = p_m.DistanceTo(p_b); //B = |P(u_m) - P(u_b)|;
            Real dist_c = p_a.DistanceTo(p_b); //C = |P(u_a) - P(u_b)|;
            Real dist_d = dist_a + dist_b - dist_c;

            Real diff_u = fabsf(segment.u_a - segment.u_b);
            if ((dist_d > length_threshold) || (diff_u > interval_threshold))
            {
                //divide segment & replace u_a, u_b
                segment_list.push_front(ArcSegment(u_m, segment.u_b));
                segment_list.push_front(ArcSegment(segment.u_a, u_m));
            }
            else
            {
                Real s_a = ArcLength(segment.u_a);
                Real s_m = s_a + dist_a; //s_m = G(u_a) + A
                Real s_b = s_m + dist_b; //s_b = G(u_m) + B

                //record u_m, s_m. record u_b, s_u.
                arc_length_table.push_back(ArcData(u_m, s_m));
                arc_length_table.push_back(ArcData(segment.u_b, s_b));
            }
        }

        //normalize s
        max_length = arc_length_table.back().arc_length;
        for (auto& arc_data : arc_length_table)
        {
            arc_data.arc_length = arc_data.arc_length / max_length;
        }
    }
}
