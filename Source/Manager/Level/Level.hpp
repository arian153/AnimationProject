#pragma once
#include <string>
#include <vector>
#include <functional>
#include "../../System/Math/Utility/MathDef.hpp"
#include "../Space/SubsystemFlag.hpp"

namespace CS460
{
    class JsonResource;
    class Space;

    class Level
    {
    public:
        explicit Level(const std::string& name);
        ~Level();

        void Initialize() const;
        void Update(Real dt) const;
        void FixedUpdate(Real dt) const;
        void Shutdown() const;
        void Load() const;
        void Unload() const;

        Space* GetGlobalSpace() const;
        Space* GetSpace(size_t index) const;
        Space* GetSpace(const std::string& name) const;

        void UpdateSpace(Real dt, size_t index, eSubsystemFlag flag) const;
        void UpdateSubsystem(Real dt, eSubsystemFlag flag) const;
        void DrawSubsystem(eSubsystemFlag flag);
        void FixedUpdateSubsystem(Real dt, eSubsystemFlag flag) const;

        void AddSpaceResource(JsonResource* resource);

    private:
        friend class LevelManager;

    private:
        void UpdateSpace(Real dt, Space* space, eSubsystemFlag flag) const;
        void RenderSpace(Space* space, eSubsystemFlag flag);
        
    private:
        Space* m_global_space = nullptr;

        eSubsystemFlag m_world_flag = eSubsystemFlag::None;
        eSubsystemFlag m_ui_flag    = eSubsystemFlag::None;

        std::vector<Space*>        m_spaces;
        std::vector<JsonResource*> m_space_resources;
        std::string                m_name = "Default";
    };
}
