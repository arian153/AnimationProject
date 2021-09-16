#include "UndefinedResource.hpp"

namespace CS460
{
    UndefinedResource::UndefinedResource(const std::wstring& path)
        : Resource(path)
    {
    }

    UndefinedResource::~UndefinedResource()
    {
    }

    void UndefinedResource::Initialize()
    {
    }

    void UndefinedResource::Shutdown()
    {
    }
}
