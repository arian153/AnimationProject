#include "Ray.hpp"
#include "../../Utility/Utility.hpp"
#include "Line.hpp"
#include "../../Structure/VQSTransform.hpp"

namespace CS460
{
    Ray::Ray(const Vector3& position, const Vector3& direction)
        : position(position), direction(direction)
    {
    }

    Ray::Ray(const Ray& rhs)
        : position(rhs.position), direction(rhs.direction)
    {
    }

    Ray::~Ray()
    {
    }

    Vector3 Ray::ParamToPoint(Real t) const
    {
        if (t < 0.0f)
        {
            return position;
        }
        return position + direction * t;
    }

    Real Ray::PointToParam(const Vector3& point_on_ray) const
    {
        Vector3 t_dir   = point_on_ray - position;
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

    Ray Ray::GetReflectedRay(const Vector3& normal, const Vector3& new_position) const
    {
        Vector3 n                 = normal.Normalize();
        Vector3 reflect_direction = -2.0f * DotProduct(direction, n) * n + direction;
        return Ray(new_position, reflect_direction.Normalize());
    }

    Vector3 Ray::SymmetricPoint(const Vector3& point) const
    {
        Vector3 closest = ClosestPoint(point);
        return closest + closest - point;
    }

    Vector3 Ray::ClosestPoint(const Vector3& point) const
    {
        Vector3 w    = point - position;
        Real    proj = w.DotProduct(direction);

        // endpoint 0 is closest point
        if (proj <= 0.0f)
        {
            return position;
        }
        // somewhere else in ray
        Real ddd = direction.DotProduct(direction);
        return position + (proj / ddd) * direction;
    }

    Real Ray::Distance(const Vector3& point, Real& t) const
    {
        return sqrtf(DistanceSquared(point, t));
    }

    Real Ray::DistanceSquared(const Vector3& point, Real& t) const
    {
        Vector3 w    = point - position;
        Real    proj = w.DotProduct(direction);

        // origin is closest point
        if (proj <= 0)
        {
            t = 0.0f;
            return w.DotProduct(w);
        }
        // else use normal line check
        Real ddd = direction.DotProduct(direction);
        t        = proj / ddd;
        return w.DotProduct(w) - t * proj;
    }

    Vector3Pair Ray::ClosestPoint(const Ray& ray) const
    {
        Vector3 w0          = position - ray.position;
        Real    r0d_dot_r0d = direction.DotProduct(direction);
        Real    r0d_dot_r1d = direction.DotProduct(ray.direction);
        Real    r1d_dot_r1d = ray.direction.DotProduct(ray.direction);
        Real    r0d_dot_w   = direction.DotProduct(w0);
        Real    r1d_dot_w   = ray.direction.DotProduct(w0);
        Real    denominator = r0d_dot_r0d * r1d_dot_r1d - r0d_dot_r1d * r0d_dot_r1d;
        // parameters to compute s_c, t_c
        Real s_c, t_c;
        Real sn,  sd, tn, td;

        // if denominator is zero, try finding closest point on ray1 to origin0
        if (Math::IsZero(denominator))
        {
            sd = td = r1d_dot_r1d;
            sn = 0.0f;
            tn = r1d_dot_w;
        }
        else
        {
            // start by clamping s_c
            sd = td = denominator;
            sn = r0d_dot_r1d * r1d_dot_w - r1d_dot_r1d * r0d_dot_w;
            tn = r0d_dot_r0d * r1d_dot_w - r0d_dot_r1d * r0d_dot_w;

            // clamp s_c to 0
            if (sn < 0.0f)
            {
                sn = 0.0f;
                tn = r1d_dot_w;
                td = r1d_dot_r1d;
            }
        }

        // clamp t_c within [0,+inf]
        // clamp t_c to 0
        if (tn < 0.0f)
        {
            t_c = 0.0f;
            // clamp s_c to 0
            if (-r0d_dot_w < 0.0f)
            {
                s_c = 0.0f;
            }
            else
            {
                s_c = -r0d_dot_w / r0d_dot_r0d;
            }
        }
        else
        {
            t_c = tn / td;
            s_c = sn / sd;
        }

        // compute closest points
        return Vector3Pair(position + s_c * direction, ray.position + t_c * ray.direction);
    }

    Ray Ray::ToLocal(const VQSTransform& transform) const
    {
        return Ray(transform.WorldToLocalPoint(position), transform.WorldToLocalVector(direction));
    }

    Ray Ray::ToWorld(const VQSTransform& transform) const
    {
        return Ray(transform.LocalToWorldPoint(position), transform.LocalToWorldVector(direction));
    }

    Ray& Ray::operator=(const Ray& rhs)
    {
        if (this != &rhs)
        {
            position  = rhs.position;
            direction = rhs.direction;
            direction.SetNormalize();
        }
        return *this;
    }

    bool Ray::operator==(const Ray& rhs) const
    {
        return rhs.position == position && rhs.direction == direction;
    }

    bool Ray::operator!=(const Ray& rhs) const
    {
        return rhs.position != position || rhs.direction != direction;
    }

    Real Distance(const Ray& ray0, const Ray& ray1, Real& s, Real& t)
    {
        return sqrtf(DistanceSquared(ray0, ray1, s, t));
    }

    Real Distance(const Ray& ray, const Vector3& point, Real& t)
    {
        return sqrtf(DistanceSquared(ray, point, t));
    }

    Real Distance(const Ray& ray, const Line& line, Real& s, Real& t)
    {
        return sqrtf(DistanceSquared(ray, line, s, t));
    }

    Real Distance(const Line& line, const Ray& ray, Real& s, Real& t)
    {
        return sqrtf(DistanceSquared(line, ray, s, t));
    }

    Real DistanceSquared(const Ray& ray0, const Ray& ray1, Real& s, Real& t)
    {
        Vector3 w0          = ray0.position - ray1.position;
        Real    r0d_dot_r0d = ray0.direction.DotProduct(ray0.direction);
        Real    r0d_dot_r1d = ray0.direction.DotProduct(ray1.direction);
        Real    r1d_dot_r1d = ray1.direction.DotProduct(ray1.direction);
        Real    r0d_dot_w   = ray0.direction.DotProduct(w0);
        Real    r1d_dot_w   = ray1.direction.DotProduct(w0);
        Real    denominator = r0d_dot_r0d * r1d_dot_r1d - r0d_dot_r1d * r0d_dot_r1d;
        Real    sn, sd, tn, td;

        // if denominator is zero, try finding closest point on ray1 to origin0
        if (Math::IsZero(denominator) == true)
        {
            // clamp s_c to 0
            sd = td = r1d_dot_r1d;
            sn = 0.0f;
            tn = r1d_dot_w;
        }
        else
        {
            // clamp s_c within [0,+inf]
            sd = td = denominator;
            sn = r0d_dot_r1d * r1d_dot_w - r1d_dot_r1d * r0d_dot_w;
            tn = r0d_dot_r0d * r1d_dot_w - r0d_dot_r1d * r0d_dot_w;

            // clamp s_c to 0
            if (sn < 0.0f)
            {
                sn = 0.0f;
                tn = r1d_dot_w;
                td = r1d_dot_r1d;
            }
        }

        // clamp t within [0,+inf]
        // clamp t to 0
        if (tn < 0.0f)
        {
            t = 0.0f;
            // clamp s_c to 0
            if (-r0d_dot_w < 0.0f)
            {
                s = 0.0f;
            }
            else
            {
                s = -r0d_dot_w / r0d_dot_r0d;
            }
        }
        else
        {
            t = tn / td;
            s = sn / sd;
        }

        // compute difference vector and distance squared
        Vector3 wc = w0 + (ray0.direction * s) - (ray1.direction * t);
        return wc.DotProduct(wc);
    }

    Real DistanceSquared(const Ray& ray, const Vector3& point, Real& t)
    {
        Vector3 w    = point - ray.position;
        Real    proj = w.DotProduct(ray.direction);
        // origin is closest point
        if (proj <= 0)
        {
            t = 0.0f;
            return w.DotProduct(w);
        }
        // else use normal line check
        Real vsq = ray.direction.DotProduct(ray.direction);
        t        = proj / vsq;
        return w.DotProduct(w) - (proj * t);
    }

    Real DistanceSquared(const Ray& ray, const Line& line, Real& s, Real& t)
    {
        Vector3 w0          = ray.position - line.position;
        Real    rd_dot_rd   = ray.direction.DotProduct(ray.direction);
        Real    rd_dot_ld   = ray.direction.DotProduct(line.direction);
        Real    ld_dot_ld   = line.direction.DotProduct(line.direction);
        Real    rd_dot_w    = ray.direction.DotProduct(w0);
        Real    ld_dot_w    = line.direction.DotProduct(w0);
        Real    denominator = rd_dot_rd * ld_dot_ld - rd_dot_ld * rd_dot_ld;

        // if denominator is zero, try finding closest point on ray1 to origin0
        if (Math::IsZero(denominator))
        {
            s = 0.0f;
            t = ld_dot_w / ld_dot_ld;
            // compute difference vector and distance squared
            Vector3 wc = w0 - t * line.direction;
            return wc.DotProduct(wc);
        }
        // clamp s_c within [0,1]
        Real sn = rd_dot_ld * ld_dot_w - ld_dot_ld * rd_dot_w;

        // clamp s_c to 0
        if (sn < 0.0f)
        {
            s = 0.0f;
            t = ld_dot_w / ld_dot_ld;
        }
            // clamp s_c to 1
        else if (sn > denominator)
        {
            s = 1.0f;
            t = (ld_dot_w + rd_dot_ld) / ld_dot_ld;
        }
        else
        {
            s = sn / denominator;
            t = (rd_dot_rd * ld_dot_w - rd_dot_ld * rd_dot_w) / denominator;
        }

        // compute difference vector and distance squared
        Vector3 wc = w0 + s * ray.direction - t * line.direction;
        return wc.DotProduct(wc);
    }

    Real DistanceSquared(const Line& line, const Ray& ray, Real& s, Real& t)
    {
        return DistanceSquared(ray, line, s, t);
    }

    Vector3Pair ClosestPoint(const Ray& ray0, const Ray& ray1)
    {
        Vector3 w0          = ray0.position - ray1.position;
        Real    r0d_dot_r0d = ray0.direction.DotProduct(ray0.direction);
        Real    r0d_dot_r1d = ray0.direction.DotProduct(ray1.direction);
        Real    r1d_dot_r1d = ray1.direction.DotProduct(ray1.direction);
        Real    r0d_dot_w   = ray0.direction.DotProduct(w0);
        Real    r1d_dot_w   = ray1.direction.DotProduct(w0);
        Real    denominator = r0d_dot_r0d * r1d_dot_r1d - r0d_dot_r1d * r0d_dot_r1d;
        // parameters to compute s_c, t_c
        Real s_c, t_c;
        Real sn,  sd, tn, td;

        // if denominator is zero, try finding closest point on ray1 to origin0
        if (Math::IsZero(denominator))
        {
            sd = td = r1d_dot_r1d;
            sn = 0.0f;
            tn = r1d_dot_w;
        }
        else
        {
            // start by clamping s_c
            sd = td = denominator;
            sn = r0d_dot_r1d * r1d_dot_w - r1d_dot_r1d * r0d_dot_w;
            tn = r0d_dot_r0d * r1d_dot_w - r0d_dot_r1d * r0d_dot_w;

            // clamp s_c to 0
            if (sn < 0.0f)
            {
                sn = 0.0f;
                tn = r1d_dot_w;
                td = r1d_dot_r1d;
            }
        }

        // clamp t_c within [0,+inf]
        // clamp t_c to 0
        if (tn < 0.0f)
        {
            t_c = 0.0f;
            // clamp s_c to 0
            if (-r0d_dot_w < 0.0f)
            {
                s_c = 0.0f;
            }
            else
            {
                s_c = -r0d_dot_w / r0d_dot_r0d;
            }
        }
        else
        {
            t_c = tn / td;
            s_c = sn / sd;
        }

        // compute closest points
        return Vector3Pair(ray0.position + s_c * ray0.direction, ray1.position + t_c * ray1.direction);
    }

    Vector3Pair ClosestPoint(const Ray& ray, const Line& line)
    {
        // compute intermediate parameters
        Vector3Pair result;
        Vector3     w0          = ray.position - line.position;
        Real        rd_dot_rd   = ray.direction.DotProduct(ray.direction);
        Real        rd_dot_ld   = ray.direction.DotProduct(line.direction);
        Real        ld_dot_ld   = line.direction.DotProduct(line.direction);
        Real        rd_dot_w    = ray.direction.DotProduct(w0);
        Real        ld_dot_w    = line.direction.DotProduct(w0);
        Real        denominator = rd_dot_rd * ld_dot_ld - rd_dot_ld * rd_dot_ld;

        // if denominator is zero, try finding closest point on ray1 to origin0
        if (Math::IsZero(denominator))
        {
            // compute closest points
            result.a = ray.position;
            result.b = line.position + (ld_dot_w / ld_dot_ld) * line.direction;
        }
        else
        {
            // parameters to compute s_c, t_c
            Real s_c, t_c;

            // clamp s_c within [0,1]
            Real sn = rd_dot_ld * ld_dot_w - ld_dot_ld * rd_dot_w;

            // clamp s_c to 0
            if (sn < 0.0f)
            {
                s_c = 0.0f;
                t_c = ld_dot_w / ld_dot_ld;
            }
                // clamp s_c to 1
            else if (sn > denominator)
            {
                s_c = 1.0f;
                t_c = (ld_dot_w + rd_dot_ld) / ld_dot_ld;
            }
            else
            {
                s_c = sn / denominator;
                t_c = (rd_dot_rd * ld_dot_w - rd_dot_ld * rd_dot_w) / denominator;
            }

            // compute closest points
            result.a = ray.position + s_c * ray.direction;
            result.b = line.position + t_c * line.direction;
        }
        return result;
    }
}
