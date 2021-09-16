#include "ComponentManager.hpp"
#include "Component.hpp"
#include "../Object/Object.hpp"
#include "ComponentRegistry.hpp"
#include "ComponentFactory.hpp"

namespace CS460
{
    ComponentManager::ComponentManager()
    {
    }

    ComponentManager::~ComponentManager()
    {
    }

    void ComponentManager::Initialize(ComponentRegistry* registry, Space* space)
    {
        m_registry = registry;
        m_space    = space;
    }

    void ComponentManager::Shutdown()
    {
        Clear();
    }

    Component* ComponentManager::Create(const std::string& type, Object* owner)
    {
        auto found = m_registry->m_factories.find(type);
        if (found != m_registry->m_factories.end())
        {
            auto created = found->second->Create(owner, m_space);
            m_components.emplace(owner, created);
            return created;
        }
        return nullptr;
    }

    Component* ComponentManager::Find(const std::string& type, Object* owner)
    {
        return owner->GetComponent(type);
    }

    Component* ComponentManager::Clone(Component* origin, Object* dest)
    {
        auto found = m_registry->m_factories.find(origin->m_type);
        if (found != m_registry->m_factories.end())
        {
            auto cloned = found->second->Clone(origin, dest, m_space);
            m_components.emplace(dest, cloned);
            return cloned;
        }
        return nullptr;
    }

    void ComponentManager::Find(const std::string& type, std::vector<Component*>& components)
    {
        auto begin = m_components.begin();
        auto end   = m_components.end();
        for (auto it = begin; it != begin; ++it)
        {
            auto found = Find(type, it->first);
            if (found != nullptr)
            {
                components.push_back(found);
            }
        }
    }

    void ComponentManager::Remove(Component* component)
    {
        auto compo_ret = m_components.equal_range(component->m_owner);
        for (auto it = compo_ret.first; it != compo_ret.second; ++it)
        {
            if (it->second == component)
            {
                it->second->Shutdown();
                delete it->second;
                it->second = nullptr;
                m_components.erase(it);
                break;
            }
        }
    }

    void ComponentManager::Remove(Component* component, Object* owner)
    {
        if (component->m_owner == owner)
        {
            auto compo_ret = m_components.equal_range(owner);
            for (auto it = compo_ret.first; it != compo_ret.second; ++it)
            {
                if (it->second == component)
                {
                    it->second->Shutdown();
                    delete it->second;
                    it->second = nullptr;
                    m_components.erase(it);
                    break;
                }
            }
        }
    }

    void ComponentManager::Remove(Object* owner)
    {
        auto compo_ret = m_components.equal_range(owner);
        for (auto it = compo_ret.first; it != compo_ret.second;)
        {
            if (it->second != nullptr)
            {
                it->second->Shutdown();
                delete it->second;
                it->second = nullptr;
            }
            m_components.erase(it++);
        }
    }

    void ComponentManager::Clear()
    {
        for (auto& component : m_components)
        {
            if (component.second != nullptr)
            {
                component.second->Shutdown();
                delete component.second;
                component.second = nullptr;
            }
        }
        m_components.clear();
    }
}
