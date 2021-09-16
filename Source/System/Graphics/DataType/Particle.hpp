#pragma once
#include "Color.hpp"
#include "../../Math/Algebra/Vector3.hpp"

namespace CS460
{
    class Particle
    {
    public:
        Particle()
        {
        }

        ~Particle()
        {
        }

        bool IsAlive() const
        {
            return life > 0.0f && scale > 0.0f;
        }

        Particle& Merge(const Particle& particle)
        {
            position += particle.position;
            velocity += particle.velocity;
            color.r += particle.color.r;
            color.g += particle.color.g;
            color.b += particle.color.b;
            color.a += particle.color.a;
            life += particle.life;
            scale += particle.scale;
            return *this;
        }

        Particle& operator=(const Particle& rhs)
        {
            if (&rhs != this)
            {
                position = rhs.position;
                velocity = rhs.velocity;
                color    = rhs.color;
                life     = rhs.life;
                scale    = rhs.scale;
                b_active = rhs.b_active;
                factor   = rhs.factor;
            }
            return *this;
        }

    public:
        Vector3 position;
        Vector3 velocity;
        Color   color;
        Real    life     = 0.0f;
        Real    scale    = 1.0f;
        Real    factor   = 0.0f;
        bool    b_active = false;
    };
}
