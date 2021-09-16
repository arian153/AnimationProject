#include "Line.hpp"
#include "../../Utility/Utility.hpp"

namespace CS460
{
    Line::Line(const Vector3& position, const Vector3& direction)
        : position(position), direction(direction)
    {
    }

    Line::Line(const Line& rhs)
        : position(rhs.position), direction(rhs.direction)
    {
    }

    Line::~Line()
    {
    }

    Vector3 Line::ParamToPoint(Real t) const
    {
        return position + direction * t;
    }

    Real Line::PointToParam(const Vector3& point_on_line) const
    {
        Vector3 t_dir   = point_on_line - position;
        Vector3 inv_dir = direction;
        inv_dir.SetInverse();
        Vector3 t_vector = t_dir.HadamardProduct(inv_dir);
        if (Math::IsValid(t_vector.x) == true)
        {
            return t_vector.x;
        }
        if (Math::IsValid(t_vector.y) == true)
        {
            return t_vector.y;
        }
        if (Math::IsValid(t_vector.z) == true)
        {
            return t_vector.z;
        }
        return 0.0f;
    }

    Line Line::GetReflectedLine(const Vector3& normal, const Vector3& new_position) const
    {
        Vector3 n                 = normal.Normalize();
        Vector3 reflect_direction = -2.0f * direction.DotProduct(n) * n + direction;
        return Line(new_position, reflect_direction);
    }

    Vector3 Line::SymmetricPoint(const Vector3& point) const
    {
        Vector3 closest = ClosestPoint(point);
        return closest + closest - point;
    }

    Vector3 Line::ClosestPoint(const Vector3& point) const
    {
        Vector3 w    = point - position;
        Real    proj = w.DotProduct(direction);
        Real    ddd  = direction.DotProduct(direction);
        return position + (proj / ddd) * direction;
    }

    Real Line::Distance(const Vector3& point, Real& t) const
    {
        return sqrtf(DistanceSquared(point, t));
    }

    Real Line::DistanceSquared(const Vector3& point, Real& t) const
    {
        Vector3 w    = point - position;
        Real    proj = w.DotProduct(direction);
        Real    ddd  = direction.DotProduct(direction);
        t            = proj / ddd;
        return w.DotProduct(w) - t * proj;
    }

    Vector3Pair Line::ClosestPoint(const Line& line) const
    {
        Vector3Pair result;
        Vector3     w0          = position - line.position;
        Real        l0d_dot_l0d = direction.DotProduct(direction);
        Real        l0d_dot_l1d = direction.DotProduct(line.direction);
        Real        l1d_dot_l1d = line.direction.DotProduct(line.direction);
        Real        l0d_dot_w   = direction.DotProduct(w0);
        Real        l1d_dot_w   = line.direction.DotProduct(w0);
        Real        denominator = l0d_dot_l0d * l1d_dot_l1d - l0d_dot_l1d * l0d_dot_l1d;
        if (Math::IsZero(denominator) == true)
        {
            result.a = position;
            result.b = line.position + (l1d_dot_w / l1d_dot_l1d) * line.direction;
        }
        else
        {
            result.a = position + ((l0d_dot_l1d * l1d_dot_w - l1d_dot_l1d * l0d_dot_w) / denominator) * direction;
            result.b = line.position + ((l0d_dot_l0d * l1d_dot_w - l0d_dot_l1d * l0d_dot_w) / denominator) * line.direction;
        }
        return result;
    }

    Line& Line::operator=(const Line& rhs)
    {
        if (this != &rhs)
        {
            position  = rhs.position;
            direction = rhs.direction;
            direction.SetNormalize();
        }
        return *this;
    }

    bool Line::operator==(const Line& rhs) const
    {
        return rhs.position == position && rhs.direction == direction;
    }

    bool Line::operator!=(const Line& rhs) const
    {
        return rhs.position != position || rhs.direction != direction;
    }
}
