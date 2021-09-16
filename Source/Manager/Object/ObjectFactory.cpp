#include "ObjectFactory.hpp"
#include "Object.hpp"
#include "ObjectManager.hpp"
#include "../Component/ComponentManager.hpp"
#include "../Resource/ResourceType/JsonResource.hpp"
#include "../../External/JSONCPP/json/json.h"
#include "../Component/Component.hpp"
#include "../Resource/ResourceManager.hpp"
#include "../Space/Space.hpp"

namespace CS460
{
    ObjectFactory::ObjectFactory()
    {
    }

    ObjectFactory::~ObjectFactory()
    {
    }

    void ObjectFactory::Initialize(ComponentRegistry* component_registry)
    {
    }

    void ObjectFactory::Shutdown()
    {
        ClearArchetypes();
    }

    Object* ObjectFactory::CreateRawObject(const std::string& name)
    {
        Object* created = new Object();
        created->m_name = name;
        return created;
    }

    Object* ObjectFactory::CreateRawObject(const std::string& name, ObjectManager* object_manager)
    {
        if (object_manager != nullptr)
        {
            return object_manager->AddObject(name);
        }
        return nullptr;
    }

    Object* ObjectFactory::CreateArchetypeObject(const std::string& name, size_t archetype_id, Space* space)
    {
        if (m_archetypes.size() > archetype_id)
        {
            auto archetype = m_archetypes.at(archetype_id)->Clone(name, space);
            return archetype;
        }
        return nullptr;
    }

    void ObjectFactory::LoadArchetype(ResourceManager* resource_manager)
    {
        std::vector<JsonResource*> resources;
        resource_manager->GetJsonResources(eJsonType::Archetype, resources);

        for (auto& resource : resources)
        {
            AddArchetype(resource);
        }
    }

    void ObjectFactory::AddArchetype(Object* object)
    {
        Object* archetype = CreateRawObject("", nullptr);
        object->CloneComponents(archetype, m_archetype_space->GetComponentManager());
        if (object->m_children != nullptr)
        {
            object->CloneChildrenRecursive(archetype, this, m_archetype_space->GetComponentManager());
            m_archetypes.push_back(object);
        }
    }

    void ObjectFactory::AddArchetype(JsonResource* resource)
    {
        if (resource->IsArchetype() == true)
        {
            auto data = *resource->m_root_data;
            if (JsonResource::HasMember(data, "Name") && (data)["Name"].isString())
            {
                std::string name   = data["Name"].asString();
                Object*     object = CreateRawObject(name);
                object->LoadArchetype(data, this, m_archetype_space);
                m_archetypes.push_back(object);
            }
        }
    }

    size_t ObjectFactory::GetArchetypeID(JsonResource* resource)
    {
        auto found = m_resource_id.find(resource);
        if (found != m_resource_id.end())
        {
            return found->second;
        }
        return m_archetypes.size() + 1;
    }

    bool ObjectFactory::ValidateID(size_t archetype_id) const
    {
        return m_archetypes.size() > archetype_id;
    }

    size_t ObjectFactory::InvalidID() const
    {
        return m_archetypes.size() + 1;
    }

    void ObjectFactory::SetArchetypeSpace(Space* space)
    {
        m_archetype_space = space;
    }

    void ObjectFactory::GetArchetypeName(std::vector<std::string>& names)
    {
        for (auto& archetype : m_archetypes)
        {
            names.push_back(archetype->GetName());
        }
    }

    void ObjectFactory::ClearArchetypes()
    {
        for (auto& archetype : m_archetypes)
        {
            archetype->RemoveObjectHierarchy();
            delete archetype;
            archetype = nullptr;
        }
        m_archetypes.clear();
    }
}
