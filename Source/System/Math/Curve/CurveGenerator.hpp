#pragma once
#include "Curve.hpp"

namespace CS460
{
    class CurveGenerator
    {
    public:

        static Curve GenerateBezierCurve(const std::vector<Vector3>& control_points, int sample = 1000);
        static Curve GeneratePolynomialCurve(const std::vector<Vector3>& control_points, int sample = 100);
        static Curve GenerateSplineCurve(const std::vector<Vector3>& control_points, int sample = 100);

    private:

        static double TruncatedCubicFunction(int idx, double t);
        static double TruncatedCubicDoublePrime(int idx, double t);
        static void   Inverse(std::vector<std::vector<double>> matrix, std::vector<std::vector<double>>& result);
    };
}
