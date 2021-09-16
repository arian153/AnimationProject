#include "FrictionUtility.hpp"

namespace CS460
{
    FrictionCoefficient::FrictionCoefficient(Real static_friction, Real dynamic_friction)
        : static_friction(static_friction), dynamic_friction(dynamic_friction)
    {
    }

    FrictionCoefficient::~FrictionCoefficient()
    {
    }

    FrictionUtility::FrictionUtility()
    {
    }

    FrictionUtility::~FrictionUtility()
    {
    }

    void FrictionUtility::Initialize()
    {
        m_friction_map.emplace(static_cast<size_t>(Physics::Material::WOOD.type) + static_cast<size_t>(Physics::Material::WOOD.type), FrictionCoefficient(0.3f, 0.2f));
        m_friction_map.emplace(static_cast<size_t>(Physics::Material::CONCRETE.type) + static_cast<size_t>(Physics::Material::WOOD.type), FrictionCoefficient(0.5f, 0.4f));
        m_friction_map.emplace(static_cast<size_t>(Physics::Material::ICE.type) + static_cast<size_t>(Physics::Material::WOOD.type), FrictionCoefficient(0.2f, 0.1f));
        m_friction_map.emplace(static_cast<size_t>(Physics::Material::GLASS.type) + static_cast<size_t>(Physics::Material::ICE.type), FrictionCoefficient(0.1f, 0.03f));
        m_friction_map.emplace(static_cast<size_t>(Physics::Material::GLASS.type) + static_cast<size_t>(Physics::Material::GLASS.type), FrictionCoefficient(0.95f, 0.4f));
        m_friction_map.emplace(static_cast<size_t>(Physics::Material::METAL.type) + static_cast<size_t>(Physics::Material::METAL.type), FrictionCoefficient(0.6f, 0.4f));
        m_friction_map.emplace(static_cast<size_t>(Physics::Material::METAL.type) + static_cast<size_t>(Physics::Material::LUBRICANT.type), FrictionCoefficient(0.1f, 0.05f));
        m_friction_map.emplace(static_cast<size_t>(Physics::Material::CONCRETE.type) + static_cast<size_t>(Physics::Material::RUBBER.type), FrictionCoefficient(1.0f, 0.8f));
        m_friction_map.emplace(static_cast<size_t>(Physics::Material::VELCRO.type) + static_cast<size_t>(Physics::Material::VELCRO.type), FrictionCoefficient(6.0f, 4.0f));
        m_friction_map.emplace(static_cast<size_t>(Physics::Material::STATIC.type) + static_cast<size_t>(Physics::Material::VELCRO.type), FrictionCoefficient(6.0f, 4.0f));
        m_friction_map.emplace(static_cast<size_t>(Physics::Material::STATIC.type) + static_cast<size_t>(Physics::Material::STATIC.type), FrictionCoefficient(1.0f, 1.0f));
    }

    void FrictionUtility::Shutdown()
    {
        m_friction_map.clear();
    }

    FrictionCoefficient FrictionUtility::Find(Physics::eMaterial a, Physics::eMaterial b)
    {
        size_t key   = static_cast<size_t>(a) + static_cast<size_t>(b);
        auto   found = m_friction_map.find(key);
        if (found != m_friction_map.end())
        {
            return found->second;
        }
        //else return default coefficient.
        return FrictionCoefficient();
    }

    FrictionCoefficient FrictionUtility::Find(Physics::MaterialData a, Physics::MaterialData b)
    {
        size_t key   = static_cast<size_t>(a.type) + static_cast<size_t>(b.type);
        auto   found = m_friction_map.find(key);
        if (found != m_friction_map.end())
        {
            return found->second;
        }
        //else return default coefficient.
        return FrictionCoefficient();
    }
}
