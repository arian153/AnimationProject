#include "SpaceManager.hpp"
#include "Space.hpp"
#include "../../System/Physics/PhysicsSystem.hpp"
#include "../Resource/ResourceType/JsonResource.hpp"
#include "../../System/Core/Utility/CoreUtility.hpp"
#include "../Object/ObjectFactory.hpp"

namespace CS460
{
    SpaceManager::SpaceManager(PhysicsSystem*     physics_system,
                               RenderSystem*      render_system,
                               ObjectFactory*     object_factory,
                               ComponentRegistry* component_registry,
                               ResourceManager*   resource_manager,
                               LogicSystem*       logic_system)
        : m_physics_system(physics_system),
          m_render_system(render_system),
          m_logic_system(logic_system),
          m_resource_manager(resource_manager),
          m_object_factory(object_factory),
          m_component_registry(component_registry)
    {
    }

    SpaceManager::~SpaceManager()
    {
    }

    void SpaceManager::Initialize()
    {
        if (m_global_space == nullptr)
        {
            m_global_space = new Space();
            m_global_flag  = eSubsystemFlag::ComponentManager | eSubsystemFlag::ObjectManager | eSubsystemFlag::Scene | eSubsystemFlag::World | eSubsystemFlag::Logic;
            m_global_space->Initialize(m_global_flag, m_physics_system, m_render_system, m_object_factory, m_component_registry, m_logic_system);
        }

        if(m_archetype_space == nullptr)
        {
            m_archetype_space = new Space();
            eSubsystemFlag flag = eSubsystemFlag::ComponentManager | eSubsystemFlag::ObjectManager | eSubsystemFlag::Scene | eSubsystemFlag::World | eSubsystemFlag::Logic;
            m_archetype_space->Initialize(flag, m_physics_system, m_render_system, m_object_factory, m_component_registry, m_logic_system);
            m_object_factory->SetArchetypeSpace(m_archetype_space);
        }
    }

    void SpaceManager::Shutdown()
    {
        for (auto& space : m_spaces)
        {
            space->Shutdown(m_physics_system, m_render_system, m_logic_system);
            delete space;
            space = nullptr;
        }
        m_spaces.clear();
        if (m_global_space != nullptr)
        {
            m_global_space->Shutdown(m_physics_system, m_render_system, m_logic_system);
            delete m_global_space;
            m_global_space = nullptr;
        }
        if (m_archetype_space != nullptr)
        {
            m_archetype_space->Shutdown(m_physics_system, m_render_system, m_logic_system);
            delete m_archetype_space;
            m_archetype_space = nullptr;
        }
    }

    Space* SpaceManager::GetGlobalSpace() const
    {
        return m_global_space;
    }

    Space* SpaceManager::CreateSpace(Level* level, eSubsystemFlag flag)
    {
        Space* space = new Space();
        m_spaces.push_back(space);
        space->m_creation_flag    = flag;
        space->m_resource_manager = m_resource_manager;
        space->m_level            = level;
        space->Initialize(flag, m_physics_system, m_render_system, m_object_factory, m_component_registry, m_logic_system);
        return space;
    }

    Space* SpaceManager::CreateSpace(Level* level, JsonResource* resource)
    {
        Space* space           = new Space();
        space->m_space_manager = this;
        m_spaces.push_back(space);
        space->m_resource_manager = m_resource_manager;
        space->m_level            = level;
        space->m_name             = resource->FileName();
        space->Initialize(resource, m_physics_system, m_render_system, m_object_factory, m_component_registry, m_logic_system);
        return space;
    }

    Space* SpaceManager::CreateSpace(JsonResource* resource)
    {
        Space* space           = new Space();
        space->m_space_manager = this;
        m_spaces.push_back(space);
        space->m_resource_manager = m_resource_manager;
        if (resource != nullptr)
        {
            space->m_name = resource->FileName();
            space->Initialize(resource, m_physics_system, m_render_system, m_object_factory, m_component_registry, m_logic_system);
        }
        else
        {
            space->Initialize(
                              eSubsystemFlag::ComponentManager | eSubsystemFlag::ObjectManager,
                              m_physics_system, m_render_system, m_object_factory, m_component_registry, m_logic_system);
        }
        return space;
    }

    void SpaceManager::RemoveSpace(Space* space)
    {
        if (space == m_global_space)
        {
            return;
        }
        if (space != nullptr)
        {
            m_spaces.erase(std::find(m_spaces.begin(), m_spaces.end(), space));
            space->Shutdown(m_physics_system, m_render_system, m_logic_system);
            delete space;
            space = nullptr;
        }
    }

    void SpaceManager::LoadSpace(Space* space, JsonResource* resource) const
    {
        if (resource != nullptr)
        {
            space->m_name = resource->FileName();
            space->Initialize(resource, m_physics_system, m_render_system, m_object_factory, m_component_registry, m_logic_system);
        }
    }

   }
