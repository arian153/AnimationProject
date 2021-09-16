#include "CurveGenerator.hpp"

#include "PascalTriangle.hpp"

namespace
{
    struct Double3
    {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
    };
}

namespace CS460
{
    Curve CurveGenerator::GenerateBezierCurve(const std::vector<Vector3>& control_points, int sample)
    {
        int size = (int)control_points.size();

        PascalTriangle triangle(size);

        Curve curve;
        curve.points.resize(sample + 1);

        int d = size - 1;
        for (int i = 0; i <= sample; ++i)
        {
            Real t = (Real)i / (Real)sample;

            curve.points[i].x = 0.0f;
            curve.points[i].y = 0.0f;
            curve.points[i].z = 0.0f;

            for (int j = 0; j < size; ++j)
            {
                Real bernstein_bezier = (Real)triangle.BinomialCoefficient(d, j) * powf(1.f - t, (Real)(d - j)) * powf(t, (Real)j);
                curve.points[i].x += control_points[j].x * bernstein_bezier;
                curve.points[i].y += control_points[j].y * bernstein_bezier;
                curve.points[i].z += control_points[j].z * bernstein_bezier;
            }
        }
        return curve;
    }

    Curve CurveGenerator::GeneratePolynomialCurve(const std::vector<Vector3>& control_points, int sample)
    {
        int size = (int)control_points.size();

        std::vector<std::vector<Double3>> newton_table;
        newton_table.resize(size);
        for (int i = 0; i < size; ++i)
        {
            newton_table[i].resize(21);
        }
        for (int i = 0; i < size; ++i)
        {
            //x(t)
            newton_table[i][0].x = (double)control_points[i].x;
            //y(t)
            newton_table[i][0].y = (double)control_points[i].y;
            //z(t)
            newton_table[i][0].z = (double)control_points[i].z;
        }
        int count = size - 1;
        int k     = 0;
        for (int i = 1; i < size; ++i)
        {
            for (int j = 0; j < count; ++j)
            {
                newton_table[j][i].x = (newton_table[j + 1][i - 1].x - newton_table[j][i - 1].x) / ((double)k + 1); //j + 1 + k - j = 1 + k
                newton_table[j][i].y = (newton_table[j + 1][i - 1].y - newton_table[j][i - 1].y) / ((double)k + 1);
                newton_table[j][i].z = (newton_table[j + 1][i - 1].z - newton_table[j][i - 1].z) / ((double)k + 1);
            }
            count--;
            k++;
        }

        Curve curve;
        if (size > 1)
        {
            int new_size = (size - 1) * sample + 1;

            curve.points.resize(new_size);

            for (int i = 0; i < size - 1; ++i)
            {
                for (int j = 0; j < sample; ++j)
                {
                    double t    = (double)j / (double)sample + (double)i;
                    double coef = 1.0f;

                    Double3 point = newton_table[0][0];
                    for (k = 1; k < size; ++k)
                    {
                        coef *= t - ((double)k - 1.0);
                        point.x = point.x + coef * newton_table[0][k].x;
                        point.y = point.y + coef * newton_table[0][k].y;
                        point.z = point.z + coef * newton_table[0][k].z;
                    }

                    int idx             = i * sample + j;
                    curve.points[idx].x = (Real)point.x;
                    curve.points[idx].y = (Real)point.y;
                    curve.points[idx].z = (Real)point.z;
                }
            }

            //last
            int final_i = size - 2;
            int final_j = sample;

            double t    = (double)final_j / (double)sample + (double)final_i;
            double coef = 1.0;

            Double3 point = newton_table[0][0];
            for (k = 1; k < size; ++k)
            {
                coef *= t - ((double)k - 1.0);
                point.x = point.x + coef * newton_table[0][k].x;
                point.y = point.y + coef * newton_table[0][k].y;
                point.z = point.z + coef * newton_table[0][k].z;
            }

            int idx             = final_i * sample + final_j;
            curve.points[idx].x = (Real)point.x;
            curve.points[idx].y = (Real)point.y;
            curve.points[idx].z = (Real)point.z;
        }

        return curve;
    }

    Curve CurveGenerator::GenerateSplineCurve(const std::vector<Vector3>& control_points, int sample)
    {
        std::vector<std::vector<double>> cubic_spline_table;

        int size       = (int)control_points.size();
        int table_size = size + 2;

        std::vector<std::vector<double>> temp;
        temp.resize(table_size);
        for (int i = 0; i < table_size; ++i)
        {
            temp[i].resize(table_size);
        }

        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < table_size; ++j)
            {
                temp[i][j] = TruncatedCubicFunction(j, (double)i);
            }
        }

        for (int i = 0; i < table_size; ++i)
        {
            temp[size][i] = TruncatedCubicDoublePrime(i, 0);
        }
        double last = (double)size - 1;
        for (int i = 0; i < table_size; ++i)
        {
            temp[size + 1][i] = TruncatedCubicDoublePrime(i, last);
        }

        Inverse(temp, cubic_spline_table);

        std::vector<Double3> spline_coef;

        Curve curve;
        if (size > 1)
        {
            int new_curve_size = (size - 1) * sample + 1;
            curve.points.resize(new_curve_size);
            spline_coef.resize(table_size);

            for (int i = 0; i < table_size; ++i)
            {
                Double3 point;
                for (int k = 0; k < size; k++)
                {
                    point.x = point.x + cubic_spline_table[i][k] * control_points[k].x;
                    point.y = point.y + cubic_spline_table[i][k] * control_points[k].y;
                    point.z = point.z + cubic_spline_table[i][k] * control_points[k].z;
                }
                spline_coef[i].x = point.x;
                spline_coef[i].y = point.y;
                spline_coef[i].z = point.z;
            }

            for (int i = 0; i < size - 1; ++i)
            {
                for (int j = 0; j < sample; ++j)
                {
                    double  t     = (double)j / (double)sample + (double)i;
                    Double3 point = spline_coef[0];
                    for (int k = 1; k < table_size; ++k)
                    {
                        point.x = point.x + TruncatedCubicFunction(k, t) * spline_coef[k].x;
                        point.y = point.y + TruncatedCubicFunction(k, t) * spline_coef[k].y;
                        point.z = point.z + TruncatedCubicFunction(k, t) * spline_coef[k].z;
                    }

                    int idx             = i * sample + j;
                    curve.points[idx].x = (Real)point.x;
                    curve.points[idx].y = (Real)point.y;
                    curve.points[idx].z = (Real)point.z;
                }
            }

            //last
            int final_i = size - 2;
            int final_j = sample;

            double  t           = (double)final_j / (double)sample + (double)final_i;
            Double3 final_point = spline_coef[0];
            for (int k = 1; k < table_size; ++k)
            {
                final_point.x = final_point.x + TruncatedCubicFunction(k, t) * spline_coef[k].x;
                final_point.y = final_point.y + TruncatedCubicFunction(k, t) * spline_coef[k].y;
                final_point.z = final_point.z + TruncatedCubicFunction(k, t) * spline_coef[k].z;
            }

            int idx             = final_i * sample + final_j;
            curve.points[idx].x = (Real)final_point.x;
            curve.points[idx].y = (Real)final_point.y;
            curve.points[idx].z = (Real)final_point.z;
        }
        return curve;
    }

    double CurveGenerator::TruncatedCubicFunction(int idx, double t)
    {
        //1, t, t^2, t^3
        if (idx <= 3)
            return pow(t, (double)idx);

        //else (t-i)^3 or 0
        double value = t - ((double)idx - 3);
        if (value <= 0.0)
            return 0.0;
        return pow(value, 3);
    }

    double CurveGenerator::TruncatedCubicDoublePrime(int idx, double t)
    {
        if (idx < 2)
            return 0.0;

        if (idx == 2)
            return 2.0;

        if (idx == 3)
            return 6 * t;

        double value = t - ((double)idx - 3);
        if (value <= 0.0)
            return 0.0;

        return 6 * value;
    }

    void CurveGenerator::Inverse(std::vector<std::vector<double>> matrix, std::vector<std::vector<double>>& result)
    {
        int n   = (int)matrix.size();
        int n_2 = n * 2;

        std::vector<std::vector<double>> temp;

        result.resize(n);
        temp.resize(n);
        for (int i = 0; i < n; ++i)
        {
            result[i].resize(n);
            temp[i].resize(n_2);
            for (int j = 0; j < n; ++j)
            {
                temp[i][j] = matrix[i][j];
            }
            for (int j = n; j < n_2; ++j)
            {
                temp[i][j] = i == (j - n) ? 1.0 : 0.0;
            }
        }

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (i != j)
                {
                    double ratio = temp[j][i] / temp[i][i];
                    for (int k = 0; k < 2 * n; ++k)
                    {
                        temp[j][k] -= ratio * temp[i][k];
                    }
                }
            }
        }
        for (int i = 0; i < n; ++i)
        {
            double a = temp[i][i];
            for (int j = 0; j < n_2; ++j)
            {
                temp[i][j] /= a;
            }
        }

        for (int i = 0; i < n; ++i)
        {
            for (int j = n; j < n_2; ++j)
            {
                int k        = j - n;
                result[i][k] = temp[i][j];
            }
        }
    }
}
