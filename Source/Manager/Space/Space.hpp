#pragma once
#include "SubsystemFlag.hpp"
#include "../../System/Math/Utility/MathDef.hpp"

namespace CS460
{
    class Level;
    class LogicSystem;
    class LogicSubsystem;
    class ResourceManager;
    class JsonResource;
    class ComponentRegistry;
    class ObjectFactory;
    class RenderSystem;
    class PhysicsSystem;
    class ObjectManager;
    class ComponentManager;
    class Scene;
    class World;
    class SpaceManager;

    class Space
    {
    public:
        Space();
        ~Space();

        void Initialize(eSubsystemFlag     flag,
                        PhysicsSystem*     physics_system,
                        RenderSystem*      render_system,
                        ObjectFactory*     obj_factory,
                        ComponentRegistry* cmp_registry,
                        LogicSystem*       logic_system);
        void Initialize(JsonResource*      m_space_resource,
                        PhysicsSystem*     physics_system,
                        RenderSystem*      render_system,
                        ObjectFactory*     obj_factory,
                        ComponentRegistry* cmp_registry,
                        LogicSystem*       logic_system);
        void Shutdown(PhysicsSystem* physics_system,
                      RenderSystem*  render_system,
                      LogicSystem*   logic_system);

        void Update(Real dt) const;
        void Update(Real dt, eSubsystemFlag flag) const;
        void Render() const;

        void UpdateSubsystem(Real dt, eSubsystemFlag flag) const;
        void RenderSubsystem(eSubsystemFlag flag) const;

        void ConnectSubsystem(ComponentManager* component_manager);
        void ConnectSubsystem(ObjectManager* object_manager);
        void ConnectSubsystem(Scene* scene);
        void ConnectSubsystem(World* world);
        void ConnectSubsystem(LogicSubsystem* logic_system);

        ObjectManager*    GetObjectManager() const;
        ComponentManager* GetComponentManager() const;
        ResourceManager*  GetResourceManager() const;
        Scene*            GetScene() const;
        World*            GetWorld() const;
        LogicSubsystem*   GetLogicSubsystem() const;

        bool IsSubsystemUpdate(eSubsystemFlag flag) const;
        bool IsSubsystemFixedUpdate(eSubsystemFlag flag) const;
        bool IsActivated() const;

        std::string GetName() const;

    private:
        friend class SpaceManager;
        friend class JsonResource;
        friend class SpaceEditor;

    private:
        std::string m_name;
        bool        m_b_activate  = true;
       
        eSubsystemFlag m_creation_flag     = eSubsystemFlag::None;
        eSubsystemFlag m_update_flag       = eSubsystemFlag::None;
        eSubsystemFlag m_fixed_update_flag = eSubsystemFlag::None;
        JsonResource*  m_space_resource    = nullptr;

        Level*            m_level             = nullptr;
        SpaceManager*     m_space_manager     = nullptr;
        ComponentManager* m_component_manager = nullptr;
        ObjectManager*    m_object_manager    = nullptr;
        ResourceManager*  m_resource_manager  = nullptr;
        Scene*            m_scene             = nullptr;
        World*            m_world             = nullptr;
        LogicSubsystem*   m_logic_subsystem   = nullptr;
    };
}
