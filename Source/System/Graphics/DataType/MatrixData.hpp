#pragma once
#include "../../Math/Algebra/Matrix44.hpp"

namespace CS460
{
    class MatrixData
    {
    public:
        MatrixData()
        {
        }

        MatrixData(const Matrix44& m, const Matrix44& v, const Matrix44& p)
            : model(m), view(v), projection(p)
        {
        }

        MatrixData(const Matrix44& v, const Matrix44& p)
            : view(v), projection(p)
        {
            model.SetIdentity();
        }

        ~MatrixData()
        {
        }

        Matrix44 GetMVPMatrix() const
        {
            return model * view * projection;
        }

    public:
        Matrix44 model;
        Matrix44 view;
        Matrix44 projection;
    };
}
