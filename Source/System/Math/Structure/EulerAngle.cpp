#include "EulerAngle.hpp"

namespace CS460
{

    EulerAngle::EulerAngle(Real x, Real y, Real z)
        : x_rotation(x), y_rotation(y), z_rotation(z)
    {
    }

    EulerAngle::EulerAngle(const EulerAngle& rhs)
        : x_rotation(rhs.x_rotation), y_rotation(rhs.y_rotation), z_rotation(rhs.z_rotation)
    {
    }

    EulerAngle::~EulerAngle()
    {
    }

    EulerAngle& EulerAngle::operator=(const EulerAngle& rhs)
    {
        if (this != &rhs)
        {
            x_rotation = rhs.x_rotation;
            y_rotation = rhs.y_rotation;
            z_rotation = rhs.z_rotation;
        }
        return *this;
    }
}