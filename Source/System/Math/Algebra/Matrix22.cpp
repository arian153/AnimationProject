
#include "Matrix22.hpp"
#include "Vector2.hpp"
#include "../Utility/Utility.hpp"
#include <ostream>
#include "../../Core/Utility/CoreUtility.hpp"

namespace CS460
{
    Matrix22::Matrix22(Real c0, Real c1, Real c2, Real c3)
    {
        data[0] = c0;
        data[1] = c1;
        data[2] = c2;
        data[3] = c2;
    }

    Matrix22::Matrix22(const Matrix22& rhs)
    {
        data[0] = rhs[0];
        data[1] = rhs[1];
        data[2] = rhs[2];
        data[3] = rhs[3];
    }

    Matrix22::~Matrix22()
    {
    }

    void Matrix22::SetRows(const Vector2& row1, const Vector2& row2)
    {
        data[0] = row1.x;
        data[1] = row1.y;
        data[2] = row2.x;
        data[3] = row2.y;
    }

    void Matrix22::SetColumns(const Vector2& col1, const Vector2& col2)
    {
        data[0] = col1.x;
        data[1] = col2.x;
        data[2] = col1.y;
        data[3] = col2.y;
    }

    void Matrix22::SetDiagonal(Real a, Real b)
    {
        data[0] = a;
        data[3] = b;
    }

    void Matrix22::SetClean()
    {
        for (size_t i = 0; i < 4; ++i)
        {
            if (Math::IsZero(data[i]))
                data[i] = 0.0f;
        }
    }

    void Matrix22::SetIdentity()
    {
        data[0] = 1.0f;
        data[1] = 0.0f;
        data[2] = 0.0f;
        data[3] = 1.0f;
    }

    void Matrix22::SetInverse()
    {
        *this = this->Inverse();
    }

    void Matrix22::SetTranspose()
    {
        *this = this->Transpose();
    }

    void Matrix22::SetZero()
    {
        data[0] = 0.0f;
        data[1] = 0.0f;
        data[2] = 0.0f;
        data[3] = 0.0f;
    }

    bool Matrix22::IsZero() const
    {
        for (size_t i = 0; i < 4; ++i)
        {
            if (Math::IsZero(data[i]) == false)
                return false;
        }
        return true;
    }

    bool Matrix22::IsIdentity() const
    {
        return Math::IsEqual(1.0f, data[0])
                && Math::IsEqual(1.0f, data[3])
                && Math::IsZero(data[1])
                && Math::IsZero(data[2]);
    }

    bool Matrix22::IsEqual(const Matrix22& rhs) const
    {
        for (size_t i = 0; i < 4; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return false;
        }
        return true;
    }

    bool Matrix22::IsNotEqual(const Matrix22& rhs) const
    {
        for (size_t i = 0; i < 4; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return true;
        }
        return false;
    }

    void Matrix22::GetRows(Vector2& row1, Vector2& row2) const
    {
        row1.x = data[0];
        row1.y = data[1];
        row2.x = data[2];
        row2.y = data[3];
    }

    Vector2 Matrix22::GetRow(size_t i) const
    {
        size_t val = i % 2;
        return Vector2(data[2 * val], data[2 * val + 1]);
    }

    void Matrix22::GetColumns(Vector2& col1, Vector2& col2) const
    {
        col1.x = data[0];
        col2.x = data[1];
        col1.y = data[2];
        col2.y = data[3];
    }

    Vector2 Matrix22::GetColumn(size_t i) const
    {
        size_t val = i % 2;
        return Vector2(data[val], data[val + 2]);
    }

    Matrix22 Matrix22::Adjoint() const
    {
        Matrix22 result;
        result.data[0] = data[3];
        result.data[1] = -data[1];
        result.data[2] = -data[2];
        result.data[3] = data[0];
        return result;
    }

    Real Matrix22::Determinant() const
    {
        return data[0] * data[3] - data[1] * data[2];
    }

    Real Matrix22::Trace() const
    {
        return data[0] + data[3];
    }

    Matrix22 Matrix22::Inverse() const
    {
        Matrix22 result;
        Real     det = data[0] * data[3] - data[1] * data[2];
        if (Math::IsZero(det))
        {
            E5_ASSERT(false, "Inverse singular matrix");
            return result;
        }
        Real inv_det   = 1.0f / det;
        result.data[0] = inv_det * data[3];
        result.data[1] = -inv_det * data[1];
        result.data[2] = -inv_det * data[2];
        result.data[3] = inv_det * data[0];
        return result;
    }

    Matrix22 Matrix22::Transpose() const
    {
        Matrix22 result;
        result.data[0] = data[0];
        result.data[1] = data[2];
        result.data[2] = data[1];
        result.data[3] = data[3];
        return result;
    }

    Matrix22 Matrix22::HadamardProduct(const Matrix22& rhs) const
    {
        return Matrix22(
                        data[0] * rhs.data[0], data[1] * rhs.data[1],
                        data[2] * rhs.data[2], data[3] * rhs.data[3]
                       );
    }

    Matrix22 Inverse(const Matrix22& mat)
    {
        return mat.Inverse();
    }

    Matrix22 Transpose(const Matrix22& mat)
    {
        return mat.Transpose();
    }

    Matrix22 HadamardProduct(const Matrix22& mat1, const Matrix22& mat2)
    {
        return mat1.HadamardProduct(mat2);
    }

    Matrix22& Matrix22::operator=(const Matrix22& rhs)
    {
        if (this != &rhs)
        {
            for (int i = 0; i < 4; ++i)
            {
                this->data[i] = rhs.data[i];
            }
        }
        return *this;
    }

    Real Matrix22::operator()(size_t i, size_t j) const
    {
        return data[2 * (i % 2) + (j % 2)];
    }

    Real& Matrix22::operator()(size_t i, size_t j)
    {
        return data[2 * (i % 2) + (j % 2)];
    }

    Real Matrix22::operator[](size_t i) const
    {
        return data[i];
    }

    Real& Matrix22::operator[](size_t i)
    {
        return data[i];
    }

    bool Matrix22::operator==(const Matrix22& rhs) const
    {
        for (size_t i = 0; i < 4; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return false;
        }
        return true;
    }

    bool Matrix22::operator!=(const Matrix22& rhs) const
    {
        for (size_t i = 0; i < 4; ++i)
        {
            if (Math::IsEqual(data[i], rhs.data[i]) == false)
                return true;
        }
        return false;
    }

    Matrix22 Matrix22::operator+(const Matrix22& rhs) const
    {
        Matrix22 result;
        for (size_t i = 0; i < 4; ++i)
        {
            result.data[i] = data[i] + rhs.data[i];
        }
        return result;
    }

    Matrix22& Matrix22::operator+=(const Matrix22& rhs)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    Matrix22 Matrix22::operator-(const Matrix22& rhs) const
    {
        Matrix22 result;
        for (size_t i = 0; i < 2; ++i)
        {
            result.data[i] = data[i] - rhs.data[i];
        }
        return result;
    }

    Matrix22& Matrix22::operator-=(const Matrix22& rhs)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    Matrix22 Matrix22::operator-() const
    {
        Matrix22 result;
        for (size_t i = 0; i < 4; ++i)
        {
            result.data[i] = -data[i];
        }
        return result;
    }

    Matrix22 Matrix22::operator*(const Matrix22& matrix) const
    {
        Matrix22 result;
        result.data[0] = data[0] * matrix.data[0] + data[1] * matrix.data[2];
        result.data[1] = data[0] * matrix.data[1] + data[1] * matrix.data[3];
        result.data[2] = data[2] * matrix.data[0] + data[3] * matrix.data[2];
        result.data[3] = data[2] * matrix.data[1] + data[3] * matrix.data[3];
        return result;
    }

    Matrix22& Matrix22::operator*=(const Matrix22& matrix)
    {
        Matrix22 result;
        result.data[0] = data[0] * matrix.data[0] + data[1] * matrix.data[2];
        result.data[1] = data[0] * matrix.data[1] + data[1] * matrix.data[3];
        result.data[2] = data[2] * matrix.data[0] + data[3] * matrix.data[2];
        result.data[3] = data[2] * matrix.data[1] + data[3] * matrix.data[3];
        for (size_t i = 0; i < 4; ++i)
        {
            data[i] = result.data[i];
        }
        return *this;
    }

    Vector2 Matrix22::operator*(const Vector2& vector) const
    {
        Vector2 result;
        result.x = data[0] * vector.x + data[1] * vector.y;
        result.y = data[2] * vector.x + data[3] * vector.y;
        return result;
    }

    Vector2 operator*(const Vector2& vector, const Matrix22& matrix)
    {
        Vector2 result;
        result.x = matrix.data[0] * vector.x + matrix.data[2] * vector.y;
        result.y = matrix.data[1] * vector.x + matrix.data[3] * vector.y;
        return result;
    }

    Matrix22 Matrix22::operator*(Real real) const
    {
        Matrix22 result;
        result.data[0] = data[0] * real;
        result.data[1] = data[1] * real;
        result.data[2] = data[2] * real;
        result.data[3] = data[3] * real;
        return result;
    }

    Matrix22& Matrix22::operator*=(Real real)
    {
        data[0] *= real;
        data[1] *= real;
        data[2] *= real;
        data[3] *= real;
        return *this;
    }

    Matrix22 operator*(Real real, const Matrix22& matrix)
    {
        Matrix22 result;
        result.data[0] = matrix.data[0] * real;
        result.data[1] = matrix.data[1] * real;
        result.data[2] = matrix.data[2] * real;
        result.data[3] = matrix.data[3] * real;
        return result;
    }

    std::ostream& operator<<(std::ostream& os, const Matrix22& rhs)
    {
        os << "|" << rhs.data[0] << ", " << rhs.data[1] << "|\n";
        os << "|" << rhs.data[2] << ", " << rhs.data[3] << "|";
        return os;
    }

    Matrix22 Matrix22::Identity()
    {
        return Matrix22();
    }

    Matrix22 Matrix22::Zero()
    {
        return Matrix22(0.0f, 0.0f, 0.0f, 0.0f);
    }
}
