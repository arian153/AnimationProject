#pragma once
#include <vector>
#include "../../../Graphics/DataType/Color.hpp"
#include "../../../Math/Utility/MathDef.hpp"
#include "../../../Math/Structure/Vector3Pair.hpp"

namespace Json
{
    class Value;
}

namespace CS460
{
    class RigidBody;
    class Force;
    class CommandRegistry;

    class ForceFactory
    {
    public:
        ForceFactory()
        {
        }

        virtual ~ForceFactory()
        {
        }

        virtual Force* Create() = 0;

    protected:
        friend class Resolution;
        friend class PhysicsSystem;
        std::string type;
    };

    class Force
    {
    public:
        Force()
        {
            unique_id++;
        }

        virtual ~Force()
        {
        }

        virtual void Initialize() = 0;
        virtual void Update(RigidBody* body, Real dt) = 0;
        virtual void Shutdown() = 0;
        virtual void Load(const Json::Value& data) = 0;
        virtual void Save(const Json::Value& data) = 0;
        virtual void Edit(CommandRegistry* registry) = 0;

        void Clear()
        {
            applied_forces.clear();
        }

    public:
        std::string              type;
        ColorFlag                draw_force;
        std::vector<Vector3Pair> applied_forces;
        inline static int        unique_id = 0;
    };
}
