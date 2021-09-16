#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeTextureArrayAPI

namespace CS460
{
    class TextureCommon;

    class TextureArrayCommon : public TextureArrayAPI
    {
    public:
        TextureArrayCommon();
        ~TextureArrayCommon();

        void PushFront(TextureCommon* texture);
        void PushBack(TextureCommon* texture);
        void Erase(TextureCommon* texture);
        void Clear();

        TextureCommon* Front() const;
        size_t         Size() const;
        void           Bind() const;

    private:
    };
}
