#pragma once
#include <vector>

namespace CS460
{
    class ParticleEmitter;

    class ParticleSystem
    {
    public:
        ParticleSystem();
        ~ParticleSystem();

    private:
        std::vector<ParticleEmitter*> m_emitters;
    };
}
