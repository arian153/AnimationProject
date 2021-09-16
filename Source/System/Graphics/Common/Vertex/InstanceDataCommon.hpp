#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeInstanceDataAPI

namespace CS460
{
    class Color;
    class Matrix44;

    class InstanceDataCommon : public InstanceDataAPI
    {
    public:
        InstanceDataCommon();
        ~InstanceDataCommon();

        InstanceDataCommon(const Matrix44& w, const Color& c);

    private:
    };
}
