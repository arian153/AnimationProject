#pragma once

namespace CS460
{
    class Bone;

    class Skeleton
    {
    public:
        Skeleton();
        ~Skeleton();



    private:
        Bone* m_root_bone = nullptr;
    };
}
