#pragma once
#include "../../System/Math/Utility/MathDef.hpp"
#include "../../System/Core/Utility/CoreDef.hpp"
// ReSharper disable once CppUnusedIncludeDirective
#include "../../External/imgui/imgui.h"

namespace Json
{
    class Value;
}

namespace CS460
{
    class CommandRegistry;
    class Space;
    class Object;

    class Component
    {
    public:
        explicit Component(Object* owner);
        virtual  ~Component();

        virtual void Initialize() = 0;
        virtual void Update(Real dt) = 0;
        virtual void Shutdown() = 0;
        virtual bool Load(const Json::Value& data) = 0;
        virtual void Save(Json::Value& data) const = 0;
        virtual void Edit(CommandRegistry* command_registry) = 0;

        String      Type() const;
        const char* TypeCStr() const;
        bool        IsLoaded() const;
        bool        IsUnloaded() const;
        Object*     GetOwner() const;
        Space*      GetSpace() const;
        void        SetSpace(Space* space);

    protected:
        virtual void Subscribe() = 0;
        virtual void Unsubscribe() = 0;

    protected:
        Object* m_owner = nullptr;
        Space*  m_space = nullptr;
        String  m_type;
        bool    m_b_loaded   = false;
        bool    m_b_unloaded = false;
        bool    m_b_open     = true;

    private:
        friend class ComponentManager;
    };
}
