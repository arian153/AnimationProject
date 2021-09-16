#pragma once
#include <unordered_map>

namespace CS460
{
    class Space;
    class ComponentManager;
    class Object;
    class ObjectFactory;

    class ObjectManager
    {
    public:
        ObjectManager();
        ~ObjectManager();

        void Initialize(ObjectFactory* object_factory, Space* space);
        void Shutdown();

        Object* AddObject(const std::string& name);
        Object* AddObject(const std::string& name, size_t archetype_id) const;
        Object* CloneObject(const std::string& name, Object* origin);

        void    FindObjects(const std::string& name, std::vector<Object*>& objects);
        Object* FindObjectBegin(const std::string& name);
        Object* FindObjectEnd(const std::string& name);
        Object* FindObjectAt(size_t id);

        void RemoveObjects(const std::string& name, bool b_remove_hierarchy = true);
        void RemoveObject(const std::string& name, bool b_remove_hierarchy = true);
        void RemoveObjectAt(size_t id, bool b_remove_hierarchy = true);
        void RemoveObject(Object* object, bool b_remove_hierarchy = true);

        Object* EraseObject(Object* object);
        Object* EraseObjectAt(size_t id);
        void    ClearObjects();

        bool HasObject(const std::string& name);
        bool HasObject(Object* object);

    private:
        void ChangeName(Object* object, const std::string& new_name);
        void ResetID();
        void EraseObject(Object* object, bool b_erase_hierarchy);

    private:
        friend class Object;
        friend class JsonResource;
        friend class SpaceEditor;

    private:
        std::unordered_multimap<std::string, Object*> m_object_map;
        std::vector<Object*>                          m_objects;

        ObjectFactory*    m_object_factory    = nullptr;
        Space* m_space = nullptr;
    };
}
