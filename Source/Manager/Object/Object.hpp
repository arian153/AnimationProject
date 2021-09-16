#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "../../System/Core/Utility/CoreDef.hpp"

namespace Json
{
    class Value;
}

namespace CS460
{
    class Space;
    class ComponentManager;
    class ObjectFactory;
    class ObjectManager;
    class Component;

    class Object
    {
    public:
        Object();
        ~Object();

        size_t GetID() const;

        std::string GetName() const;
        const char* GetNameChar() const;
        void        SetName(const std::string& name);

        Object* Clone(const std::string& name = "", Space* space = nullptr);

        Object* GetChildAt(size_t index) const;
        Object* GetSiblingAt(size_t index) const;
        Object* GetParent() const;
        Object* GetAncestor() const;

        std::vector<Object*>* GetChildren() const;
        std::vector<Object*>* GetSibling() const;

        void AddChild(Object* baby_child);
        void RemoveChild(Object* child) const;
        void RemoveSibling(Object* sibling) const;
        void EraseObjectHierarchy();
        void RemoveObjectHierarchy();
        void ClearComponents();

        bool Load(const Json::Value& data);
        bool Save(Json::Value& data) const;

    public: //template member functions
        template <typename T>
        T* AddComponent();

        template <typename T>
        bool HasComponent() const;

        template <typename T>
        T* GetComponent() const;

        template <typename T>
        void RemoveComponent();

    private:
        void SetManager(ObjectManager* object_manager);
        void SetManager(ComponentManager* component_manager);
        void SetFactory(ObjectFactory* factory);

        Component* AddComponent(Component* component);
        Component* AddComponent(const std::string& type);
        Component* GetComponent(const std::string& type);

        bool HasComponent(const std::string& type);
        void RemoveComponent(Component* component);
        void RemoveComponent(const std::string& type);

        void ClearObjectHierarchy();
        void RemoveChildrenRecursive();

        bool CloneComponents(Object* cloned_object, ComponentManager* manager) const;
        bool CloneHierarchy(Object* cloned_object, ObjectManager* obj, ComponentManager* cmp) const;
        void CloneChildrenRecursive(Object* cloned_object, ObjectManager* obj, ComponentManager* cmp) const;
        void CloneChildrenRecursive(Object* cloned_object, ObjectFactory* obj, ComponentManager* cmp) const;

        bool LoadArchetype(const Json::Value& data, ObjectFactory* obj_factory, Space* archetype_space);
    private:
        friend class ComponentManager;
        friend class ObjectManager;
        friend class ObjectFactory;
        friend class SpaceEditor;

    private:
        //identifier
        size_t      m_id = Core::I64_MAX;
        std::string m_name;

        //family
        Object*               m_ancestor = nullptr;
        Object*               m_parent   = nullptr;
        std::vector<Object*>* m_children = nullptr;
        std::vector<Object*>* m_siblings = nullptr;

        //components
        std::unordered_map<std::string, Component*> m_component_map;
        std::vector<Component*>                     m_components;

        //others
        ObjectManager*    m_object_manager    = nullptr;
        ComponentManager* m_component_manager = nullptr;
    };
}

// ReSharper disable once CppUnusedIncludeDirective
#include "ObjectTemplate.cpp"
