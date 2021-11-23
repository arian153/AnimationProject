#include "ComponentRegistry.hpp"
#include "ComponentFactory.hpp"

//factories
#include "EngineFactory/AniMeshFactory.hpp"
#include "EngineFactory/CameraFactory.hpp"
#include "EngineFactory/ColliderFactory.hpp"
#include "EngineFactory/IKFactory.hpp"
#include "EngineFactory/RigidBodyFactory.hpp"
#include "EngineFactory/TransformFactory.hpp"
#include "EngineFactory/MeshFactory.hpp"
#include "EngineFactory/LightFactory.hpp"
#include "EngineFactory/TextSpriteFactory.hpp"
#include "EngineFactory/ParticleEmitterFactory.hpp"
#include "EngineFactory/PointConstraintFactory.hpp"
#include "EngineFactory/SkyFactory.hpp"

namespace CS460
{
    ComponentRegistry::ComponentRegistry()
    {
    }

    ComponentRegistry::~ComponentRegistry()
    {
    }

    bool ComponentRegistry::Initialize()
    {
        AddFactory(new TransformFactory());
        AddFactory(new RigidBodyFactory());
        AddFactory(new ColliderFactory());
        AddFactory(new CameraFactory());
        AddFactory(new MeshFactory());
        AddFactory(new LightFactory());
        AddFactory(new TextSpriteFactory());
        AddFactory(new ParticleEmitterFactory());
        AddFactory(new PointConstraintFactory());
        AddFactory(new SkyFactory());
        AddFactory(new AniMeshFactory());
        AddFactory(new IKFactory());

        return true;
    }

    bool ComponentRegistry::Shutdown()
    {
        for (auto it = m_factories.begin(); it != m_factories.end(); ++it)
        {
            delete it->second;
            it->second = nullptr;
        }
        m_keys.clear();
        m_factories.clear();
        return true;
    }

    bool ComponentRegistry::AddFactory(ComponentFactory* factory)
    {
        if (factory != nullptr)
        {
            if (m_factories.find(factory->type) == m_factories.end())
            {
                m_keys.push_back(factory->type);
                m_factories.emplace(factory->type, factory);
                return true;
            }
        }
        return false;
    }
}
