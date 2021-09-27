#pragma once

#include "../../Core/Utility/CoreDef.hpp"
#include "../Common/Shader/ShaderType.hpp"
#include "../Common/Texture/TextureArrayCommon.hpp"
#include "../DataType/MaterialData.hpp"

namespace CS460
{
    class AniMesh
    {
    public:
        AniMesh();
        ~AniMesh();

        void Initialize();
        void Shutdown();

        void Bind() const;
        void Draw() const;
        void CreateBuffer();

    private:

    };
}
