#pragma once

#include "../../../Math/Algebra/Matrix44.hpp"
#include "../../../Math/Algebra/Vector3.hpp"
#include "../../../Math/Algebra/Vector2.hpp"
#include "../../../Math/Utility/MathDef.hpp"
#include "../../DataType/Color.hpp"

namespace CS460
{
    struct ColorBufferData
    {
        Color color;
    };

    struct CameraBufferData
    {
        Vector3 camera_position;
        Real    padding;
    };

    struct GammaBufferData
    {
        Real    gamma_correction;
        Vector3 padding;
    };

    struct LightBufferData
    {
        Color ambient_color;
        Color diffuse_color;
        Color specular_color;
        Color ambient_range;

        Vector3 direction;
        Real    range;

        Vector3 position;
        Real    length;

        Vector3 attenuation;
        Real    spot;

        int     type;
        Real    intensity;
        Vector2 padding;
    };

    struct MultipleLightsBufferData
    {
        LightBufferData data[16];
        int             light_count;
        Vector3         padding;
    };

    struct MatrixBufferDataInstancing
    {
        Matrix44 view;
        Matrix44 proj;
    };

    struct MatrixBufferData
    {
        Matrix44 world;
        Matrix44 view;
        Matrix44 proj;
    };

    struct InstanceBufferData
    {
        Matrix44 model;
        Color    ambient;
        Color    diffuse;
        Color    specular;
        Color    reflect;
    };

    struct TextureBufferData
    {
        int   diff_type;
        int   spec_type;
        int   norm_type;
        float gamma;
    };
}
