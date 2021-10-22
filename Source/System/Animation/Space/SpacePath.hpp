#pragma once
#include <vector>

#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Curve/BezierCurve.hpp"
#include "../../Math/Curve/Curve.hpp"

namespace CS460
{
    class PrimitiveRenderer;

    struct ArcData
    {
        Real u = 0.0f;
        Real s = 0.0f;
    };

    class SpacePath
    {
    public:
        SpacePath();
        ~SpacePath();

        void CreateCurve();
        void Update();
        void BuildTable();

        Real InverseArcLength(Real s);
        Real ArcLength(Real u);

        //s = V(t)
        //u = G(s)
        //p = P(u);

        void Draw(PrimitiveRenderer* renderer) const;
        void AddControlPoint(const Vector3& control_point);

        void SetSampleCount(size_t count);
        void GenerateCurve(); //curve for rendering

    private:
        friend class Skeleton;
        friend class AnimationSpace;

    private:
        Curve  interpolated_curve;
        size_t sample_count = 1000;
        Real   sample_step  = 1.0f / 1000.0f;
        bool   b_update = false;

        std::vector<Vector3>          control_points;
        std::vector<CubicBezierCurve> segments;
        std::vector<ArcData>          arc_length_table;
    };
}
