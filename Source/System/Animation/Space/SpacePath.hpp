#pragma once
#include <vector>

#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Curve/BezierCurve.hpp"
#include "../../Math/Curve/Curve.hpp"

namespace CS460
{
    class PrimitiveRenderer;

    struct ArcSegment
    {
        ArcSegment() = default;
        ArcSegment(Real a, Real b);

        Real u_a = 0.0f;
        Real u_b = 1.0f;
    };

    struct ArcData
    {
        ArcData() = default;
        ArcData(Real u, Real s);

        Real arc_param  = 0.0f;
        Real arc_length = 0.0f;
    };

    class SpacePath
    {
    public:
        SpacePath();
        ~SpacePath();

        void UpdateStatus();

        void Draw(PrimitiveRenderer* renderer) const;
        void AddControlPoint(const Vector3& control_point);
        void SetRenderingCurveSampleCount(size_t count);

        Vector3 SpaceCurve(Real u) const;   //p = P(u);
        Real    ArcLength(Real u) const;    //s = G(u)
        Real    InvArcLength(Real s) const; //u = InvG(s)

    private:
        void SetUpSegments();
        void GenerateRenderingCurve(); //curve for rendering
        void AdaptiveApproach();

    private:
        friend class Skeleton;
        friend class AnimationSpace;

    private:
        Curve  interpolated_curve;
        size_t sample_count = 1000;
        bool   b_update     = false;
        bool   b_was_update = false;
        bool   b_draw       = true;

        Real length_threshold   = 0.005f;
        Real interval_threshold = 0.25f;
        Real max_length         = 0.0f;

        std::vector<Vector3>          control_points;
        std::vector<CubicBezierCurve> segments;
        std::vector<ArcData>          arc_length_table;
    };
}
