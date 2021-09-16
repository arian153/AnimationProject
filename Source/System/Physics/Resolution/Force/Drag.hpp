#pragma once
#include "Force.hpp"
#include "../../../Math/Math.hpp"
#include <vector>

namespace Json
{
    class Value;
}

namespace CS460
{
    class CommandRegistry;

    class DragFactory final : public ForceFactory
    {
    public:
        DragFactory();
        ~DragFactory();
        Force* Create() override;
    };

    class Drag final : public Force
    {
    public:
        Drag();
        ~Drag();

        void SetAngularDrag(Real drag);
        void SetLinearDrag(Real drag);

        void Initialize() override;
        void Update(RigidBody* body, Real dt) override;
        void Shutdown() override;
        void Load(const Json::Value& data) override;
        void Save(const Json::Value& data) override;
        void Edit(CommandRegistry* registry) override;

    private:
        Real linear_drag  = 0.25f;
        Real angular_drag = 0.25f;
    };
}
