#pragma once
#include <string>

#include "Color.hpp"

namespace CS460
{
    struct MaterialIdentifier
    {
    public:

        bool operator==(const MaterialIdentifier& rhs) const
        {
            return (diffuse_type == rhs.diffuse_type) &&
                    (specular_type == rhs.specular_type) &&
                    (normal_type == rhs.normal_type) &&
                    (diffuse0 == rhs.diffuse0) &&
                    (diffuse1 == rhs.diffuse1) &&
                    (diffuse2 == rhs.diffuse2) &&
                    (specular0 == rhs.specular0) &&
                    (normal0 == rhs.normal0) &&
                    (shader_type == rhs.shader_type);
        }

    public:
        int         diffuse_type  = -1;
        int         specular_type = -1;
        int         normal_type   = -1;
        std::string diffuse0      = "";
        std::string diffuse1      = "";
        std::string diffuse2      = "";
        std::string specular0     = "";
        std::string normal0       = "";
        std::string shader_type   = "";
    };

    struct MaterialColor
    {
        bool operator==(const MaterialColor& rhs) const
        {
            return (ambient == rhs.ambient) &&
                    (diffuse == rhs.diffuse) &&
                    (specular == rhs.specular);
        }

        Color ambient;
        Color diffuse;
        Color specular;
        Color reflect;
    };
}
