#pragma once
#include <string>
#include "ResourceType.hpp"

namespace CS460
{
    class ResourceManager;

    class Resource
    {
    public:
        explicit Resource(const std::wstring& path);
        virtual  ~Resource();

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        bool IsLoaded() const;
        bool IsUnloaded() const;

        std::wstring WFileName() const;
        std::wstring WFilePath() const;
        std::wstring WFileType() const;
        std::string  FileName() const;
        std::string  FilePath() const;
        std::string  FileType() const;

    protected:
        std::wstring     m_file_path_w;
        std::wstring     m_file_name_w;
        std::wstring     m_file_type_w;
        std::string      m_file_path_m;
        std::string      m_file_name_m;
        std::string      m_file_type_m;
        eResourceType    m_type             = eResourceType::Undefined;
        bool             m_b_loaded         = false;
        bool             m_b_unloaded       = false;
        ResourceManager* m_resource_manager = nullptr;
    private:
        friend class ResourceManager;
    };
}
