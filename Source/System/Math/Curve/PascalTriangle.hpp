#pragma once
#include <vector>

namespace CS460
{
    class PascalTriangle
    {
    public:
        explicit PascalTriangle(int max);
        ~PascalTriangle();
        void CalculateTriangle();

        int BinomialCoefficient(int d, int i);

    public:
        int max = -1;

        std::vector<std::vector<int>> pascal_triangle_table;
    };
}
