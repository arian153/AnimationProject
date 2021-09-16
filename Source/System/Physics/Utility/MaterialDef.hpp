#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    namespace Physics
    {
        enum class eMaterial
        {
            Rock = 1,
            Wood = 2,
            Metal = 4,
            BouncyBall = 8,
            SuperBall = 16,
            Pillow = 32,
            Static = 64,
            Concrete = 128,
            Ice = 256,
            Glass = 512,
            Lubricant = 1024,
            Rubber = 2048,
            Velcro = 4096,
            UserType = 8192
        };

        class MaterialData
        {
        public:
            explicit MaterialData(eMaterial type = eMaterial::Rock, Real user_density = 1.0f, Real user_restitution = 1.0f);
            ~MaterialData();

        public:
            eMaterial type;
            Real      density;
            Real      restitution;
        };

        inline MaterialData::MaterialData(eMaterial type, Real user_density, Real user_restitution)
            : type(type)
        {
            switch (type)
            {
            case eMaterial::Rock:
                density = 0.6f;
                restitution = 0.4f;
                break;
            case eMaterial::Wood:
                density = 0.3f;
                restitution = 0.2f;
                break;
            case eMaterial::Metal:
                density = 1.2f;
                restitution = 0.05f;
                break;
            case eMaterial::BouncyBall:
                density = 0.3f;
                restitution = 0.8f;
                break;
            case eMaterial::SuperBall:
                density = 0.3f;
                restitution = 0.95f;
                break;
            case eMaterial::Pillow:
                density = 0.1f;
                restitution = 0.2f;
                break;
            case eMaterial::Static:
                density = 0.0f;
                restitution = 0.4f;
                break;
            case eMaterial::Concrete:
                density = 2.4f;
                restitution = 0.4f;
                break;
            case eMaterial::Ice:
                density = 0.9167f;
                restitution = 0.4f;
                break;
            case eMaterial::Glass:
                density = 2.4f;
                restitution = 0.4f;
                break;
            case eMaterial::Lubricant:
                density = 0.7f;
                restitution = 0.4f;
                break;
            case eMaterial::Rubber:
                density = 1.522f;
                restitution = 0.4f;
                break;
            case eMaterial::Velcro:
                density = 1.52f;
                restitution = 0.0f;
                break;
            case eMaterial::UserType:
                density = user_density;
                restitution = user_restitution;
                break;
            default:
                density = 0.6f;
                restitution = 0.4f;
                break;
            }
        }

        inline MaterialData::~MaterialData()
        {
        }

        namespace Material
        {
            const MaterialData ROCK        = MaterialData(eMaterial::Rock);
            const MaterialData WOOD        = MaterialData(eMaterial::Wood);
            const MaterialData METAL       = MaterialData(eMaterial::Metal);
            const MaterialData BOUNCY_BALL = MaterialData(eMaterial::BouncyBall);
            const MaterialData SUPER_BALL  = MaterialData(eMaterial::SuperBall);
            const MaterialData PILLOW      = MaterialData(eMaterial::Pillow);
            const MaterialData STATIC      = MaterialData(eMaterial::Static);
            const MaterialData CONCRETE    = MaterialData(eMaterial::Concrete);
            const MaterialData ICE         = MaterialData(eMaterial::Ice);
            const MaterialData GLASS       = MaterialData(eMaterial::Glass);
            const MaterialData LUBRICANT   = MaterialData(eMaterial::Lubricant);
            const MaterialData RUBBER      = MaterialData(eMaterial::Rubber);
            const MaterialData VELCRO      = MaterialData(eMaterial::Velcro);
        }
    }
}
