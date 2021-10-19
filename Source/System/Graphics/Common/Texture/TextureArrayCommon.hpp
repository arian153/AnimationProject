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
        void Replace(TextureCommon* texture, size_t idx);
        void Clear();

        TextureCommon* Front() const;
        size_t         Size() const;
        void           Bind() const;

    private:
    };
}
