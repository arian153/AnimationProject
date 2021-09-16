#pragma once
#include <locale>

namespace CS460
{
    using U8 = uint8_t;
    using U16 = uint16_t;
    using U32 = uint32_t;
    using U64 = uint64_t;
    using I8 = int8_t;
    using I16 = int16_t;
    using I32 = int32_t;
    using I64 = int64_t;
    using R32 = float;
    using R64 = double;

    using String = std::string;
    using WString = std::wstring;

    namespace Core
    {
        constexpr R32 R32_MIN = 1.175494351e-38F;
        constexpr R32 R32_MAX = 3.402823466e+38F;

        constexpr R64 R64_MIN = 2.2250738585072014e-308;
        constexpr R64 R64_MAX = 1.7976931348623158e+308;

        constexpr U8 U8_MAX = 0xffui8;
        constexpr U16 U16_MAX = 0xffffui16;
        constexpr U32 U32_MAX = 0xffffffffui32;
        constexpr U64 U64_MAX = 0xffffffffffffffffui64;

        constexpr I8 I8_MIN = (-127i8 - 1);
        constexpr I8 I8_MAX = 127i8;

        constexpr I16 I16_MIN = -32767i16 - 1;
        constexpr I16 I16_MAX = 32767i16;

        constexpr I32 I32_MIN = (-2147483647i32 - 1);
        constexpr I32 I32_MAX = 2147483647i32;

        constexpr I64 I64_MIN = (-9223372036854775807i64 - 1);
        constexpr I64 I64_MAX = 9223372036854775807i64;
    }

}