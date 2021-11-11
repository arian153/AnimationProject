#include "Manipulator.hpp"

#include "Bone.hpp"

namespace CS460
{
    Manipulator::Manipulator()
    {
    }

    Manipulator::~Manipulator()
    {
    }

    void Manipulator::SetUpLinks(Bone* end_effector)
    {
        if (!end_effector->IsEndEffector())
        {
            return;
        }

        links.clear();
        std::vector<Bone*> to_parents;
        to_parents.push_back(end_effector);

        while (!to_parents.empty())
        {
            Bone* link = to_parents.back();
            to_parents.pop_back();
            links.push_back(link);

            Bone* parent = link->GetParent();
            if (parent != nullptr)
            {
                to_parents.push_back(parent);
            }
        }
    }
}
