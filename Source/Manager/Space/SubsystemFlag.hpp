#pragma once

namespace CS460
{
    enum class eSubsystemFlag : size_t
    {
        None = 0x00,
        ComponentManager = 0x1,
        ObjectManager = 0x2,
        Scene = 0x4,
        World = 0x8,
        Logic = 0x16,
        Animation = 0x32
    };

    constexpr eSubsystemFlag operator^(eSubsystemFlag a, eSubsystemFlag b)
    {
        return static_cast<eSubsystemFlag>(static_cast<size_t>(a) ^ static_cast<size_t>(b));
    }

    constexpr eSubsystemFlag operator~(eSubsystemFlag flag)
    {
        return static_cast<eSubsystemFlag>(~static_cast<size_t>(flag));
    }

    constexpr eSubsystemFlag operator&(eSubsystemFlag a, eSubsystemFlag b)
    {
        return static_cast<eSubsystemFlag>(static_cast<size_t>(a) & static_cast<size_t>(b));
    }

    constexpr eSubsystemFlag operator|(eSubsystemFlag a, eSubsystemFlag b)
    {
        return static_cast<eSubsystemFlag>(static_cast<size_t>(a) | static_cast<size_t>(b));
    }

    inline eSubsystemFlag& operator&=(eSubsystemFlag& a, eSubsystemFlag b)
    {
        a = a & b;
        return a;
    }

    inline eSubsystemFlag& operator|=(eSubsystemFlag& a, eSubsystemFlag b)
    {
        a = a | b;
        return a;
    }

    inline eSubsystemFlag& operator^=(eSubsystemFlag& a, eSubsystemFlag b)
    {
        a = a ^ b;
        return a;
    }

    inline bool HasFlag(eSubsystemFlag a, eSubsystemFlag b)
    {
        return (a & b) == b;
    }
}
