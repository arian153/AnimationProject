#pragma once

namespace CS460
{
    enum class eLightType
    {
        AmbientLight,
        DirectionalLight,
        PointLight,
        SpotLight,
        CapsuleLight
    };

    enum class eLightingMethod
    {
        None,
        Forward,
        Deferred
    };
}
