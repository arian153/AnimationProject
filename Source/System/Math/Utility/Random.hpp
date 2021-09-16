#pragma once
#include <random>
#include "../../Core/Utility/CoreDef.hpp"
#include "MathDef.hpp"

namespace CS460
{
    class Random
    {
    public:
        Random();
        Random(size_t seed);
        ~Random();

        void SetSeed(size_t seed);
        size_t GetSeed() const;
        size_t GenerateSeed();

    public:
        unsigned GetRandomUnsigned();
        int GetRandomInteger();
        Real GetRandomReal();

        unsigned GetRangedRandomUnsigned(unsigned min, unsigned max);
        int GetRangedRandomInteger(int min, int max);
        Real GetRangedRandomReal(Real min, Real max);

        bool IsNPercentSuccess(Real percent);
        size_t NPercentSuccessMTimesResult(size_t m, Real percent);

        unsigned GetNormalDistributionUnsigned(unsigned average, unsigned deviation);
        int GetNormalDistributionInteger(int average, int deviation);
        Real GetNormalDistributionReal(Real average, Real deviation);

    public:
        U32 GetRandomU32();
        U64 GetRandomU64();
        I32 GetRandomI32();
        I64 GetRandomI64();
        R32 GetRandomR32();
        R64 GetRandomR64();

        U32 GetRangedRandomU32(U32 min, U32 max);
        U64 GetRangedRandomU64(U64 min, U64 max);
        I32 GetRangedRandomI32(I32 min, I32 max);
        I64 GetRangedRandomI64(I64 min, I64 max);
        R32 GetRangedRandomR32(R32 min, R32 max);
        R64 GetRangedRandomR64(R64 min, R64 max);

        bool IsNPercentSuccessR32(R32 percent);
        bool IsNPercentSuccessR64(R64 percent);

        U32 GetNormalDistributionU32(U32 average, U32 deviation);
        U64 GetNormalDistributionU64(U64 average, U64 deviation);
        I32 GetNormalDistributionI32(I32 average, I32 deviation);
        I64 GetNormalDistributionI64(I64 average, I64 deviation);
        R32 GetNormalDistributionR32(R32 average, R32 deviation);
        R64 GetNormalDistributionR64(R64 average, R64 deviation);

    private:
        bool m_b_64bit = false;
        size_t m_seed;
        std::mt19937 m_random32;
        std::mt19937_64 m_random64;
        std::random_device m_rng;
    };

}