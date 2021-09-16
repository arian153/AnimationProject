#include "Random.hpp"

namespace CS460
{
    Random::Random()
    {
        GenerateSeed();
    }

    Random::Random(size_t seed)
    {
        m_seed = seed;
        m_random32 = std::mt19937((U32)seed);
        m_random64 = std::mt19937_64((U64)seed);
    }

    Random::~Random()
    {
    }

    void Random::SetSeed(size_t seed)
    {
        m_seed = seed;
        m_random32 = std::mt19937((U32)seed);
        m_random64 = std::mt19937_64((U64)seed);
    }

    size_t Random::GetSeed() const
    {
        return m_seed;
    }

    size_t Random::GenerateSeed()
    {
        m_seed = m_rng();
        m_random32 = std::mt19937((U32)m_seed);
        m_random64 = std::mt19937_64((U64)m_seed);

        return m_seed;
    }

    unsigned Random::GetRandomUnsigned()
    {
        return m_random32();
    }

    int Random::GetRandomInteger()
    {
        return static_cast<int>(m_random32());
    }

    Real Random::GetRandomReal()
    {
        return static_cast<Real>(m_random32()) / static_cast<Real>(m_random32.max());
    }

    unsigned Random::GetRangedRandomUnsigned(unsigned min, unsigned max)
    {
        std::uniform_int_distribution<unsigned> dist(min, max);
        return dist(m_random32);
    }

    int Random::GetRangedRandomInteger(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(m_random32);
    }

    Real Random::GetRangedRandomReal(Real min, Real max)
    {
        std::uniform_real_distribution<Real> dist(min, max);
        return dist(m_random32);
    }

    bool Random::IsNPercentSuccess(Real percent)
    {
        std::bernoulli_distribution dist(percent);
        return dist(m_random32);
    }

    size_t Random::NPercentSuccessMTimesResult(size_t m, Real percent)
    {
        std::binomial_distribution<size_t> dist(m, percent);
        return static_cast<size_t>(dist(m_random32));
    }

    unsigned Random::GetNormalDistributionUnsigned(unsigned average, unsigned deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<unsigned>(dist(m_random32));
    }

    int Random::GetNormalDistributionInteger(int average, int deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<int>(dist(m_random32));
    }

    Real Random::GetNormalDistributionReal(Real average, Real deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<Real>(dist(m_random32));
    }

    U32 Random::GetRandomU32()
    {
        return m_random32();
    }

    U64 Random::GetRandomU64()
    {
        return m_random64();
    }

    I32 Random::GetRandomI32()
    {
        return static_cast<I32>(m_random32());
    }

    I64 Random::GetRandomI64()
    {
        return static_cast<I64>(m_random64());
    }

    R32 Random::GetRandomR32()
    {
        return static_cast<R32>(m_random32()) / static_cast<R32>(m_random32.max());
    }

    R64 Random::GetRandomR64()
    {
        return static_cast<R64>(m_random64()) / static_cast<R64>(m_random64.max());
    }

    U32 Random::GetRangedRandomU32(U32 min, U32 max)
    {
        std::uniform_int_distribution<U32> dist(min, max);
        return dist(m_random32);
    }

    U64 Random::GetRangedRandomU64(U64 min, U64 max)
    {
        std::uniform_int_distribution<U64> dist(min, max);
        return dist(m_random64);
    }

    I32 Random::GetRangedRandomI32(I32 min, I32 max)
    {
        std::uniform_int_distribution<I32> dist(min, max);
        return dist(m_random32);
    }

    I64 Random::GetRangedRandomI64(I64 min, I64 max)
    {
        std::uniform_int_distribution<I64> dist(min, max);
        return dist(m_random64);
    }

    R32 Random::GetRangedRandomR32(R32 min, R32 max)
    {
        std::uniform_real_distribution<R32> dist(min, max);
        return dist(m_random64);
    }

    R64 Random::GetRangedRandomR64(R64 min, R64 max)
    {
        std::uniform_real_distribution<R64> dist(min, max);
        return dist(m_random64);
    }

    bool Random::IsNPercentSuccessR32(R32 percent)
    {
        std::bernoulli_distribution dist(percent);
        return dist(m_random32);
    }

    bool Random::IsNPercentSuccessR64(R64 percent)
    {
        std::bernoulli_distribution dist(percent);
        return dist(m_random64);
    }

    U32 Random::GetNormalDistributionU32(U32 average, U32 deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<U32>(dist(m_random32));
    }

    U64 Random::GetNormalDistributionU64(U64 average, U64 deviation)
    {
        std::normal_distribution<> dist(static_cast<R64>(average), static_cast<R64>(deviation));
        return static_cast<U64>(dist(m_random64));
    }

    I32 Random::GetNormalDistributionI32(I32 average, I32 deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<I32>(dist(m_random32));
    }

    I64 Random::GetNormalDistributionI64(I64 average, I64 deviation)
    {
        std::normal_distribution<> dist(static_cast<R64>(average), static_cast<R64>(deviation));
        return static_cast<I64>(dist(m_random64));
    }

    R32 Random::GetNormalDistributionR32(R32 average, R32 deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<R32>(dist(m_random32));
    }

    R64 Random::GetNormalDistributionR64(R64 average, R64 deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return dist(m_random64);
    }

}