#include "Resource.hpp"

namespace CS460
{
    Resource::Resource(const std::wstring& path)
        : m_file_path_w(path)
    {
    }

    Resource::~Resource()
    {
    }

    bool Resource::IsLoaded() const
    {
        return m_b_loaded;
    }

    bool Resource::IsUnloaded() const
    {
        return m_b_unloaded;
    }

    std::wstring Resource::WFileName() const
    {
        return m_file_name_w;
    }

    std::wstring Resource::WFilePath() const
    {
        return m_file_path_w;
    }

    std::wstring Resource::WFileType() const
    {
        return m_file_type_w;
    }

    std::string Resource::FileName() const
    {
        return m_file_name_m;
    }

    std::string Resource::FilePath() const
    {
        return m_file_path_m;
    }

    std::string Resource::FileType() const
    {
        return m_file_type_m;
    }
}
