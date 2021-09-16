#include "FileUtility.hpp"
#include <filesystem>
#include <fstream>

namespace CS460
{
    FileUtility::FileUtility()
    {
    }

    FileUtility::~FileUtility()
    {
    }

    std::wstring FileUtility::GetFileNameFromPath(const std::wstring& path)
    {
        std::wstring                  filename = path.substr(path.find_last_of(L"/\\") + 1);
        const std::wstring::size_type index_of_name(filename.find_last_of('.'));
        if (index_of_name > 0 && index_of_name != std::wstring::npos)
        {
            return filename.substr(0, index_of_name);
        }
        return filename;
    }

    std::wstring FileUtility::GetFileTypeFromPath(const std::wstring& path)
    {
        std::wstring                  filename = path.substr(path.find_last_of(L"/\\") + 1);
        const std::wstring::size_type index_of_name(filename.find_last_of('.'));
        if (index_of_name > 0 && index_of_name != std::wstring::npos)
        {
            return filename.substr(index_of_name, std::wstring::npos);
        }
        return filename;
    }

    std::wstring FileUtility::GetParentPathFromPath(const std::wstring& path)
    {
        return path.substr(0, path.find_last_of(L"/\\") + 1);
    }

    void FileUtility::GetFileListsInPath(const std::wstring& path, std::list<std::wstring>& list) const
    {
        if (IsExist(path))
        {
            for (auto& iterated_path : std::filesystem::directory_iterator(path))
            {
                std::wstring str_path = iterated_path.path().generic_wstring();
                list.emplace_back(str_path);
            }
        }
    }

    void FileUtility::GetFileListsInPathRecursive(const std::wstring& path, std::list<std::wstring>& list)
    {
        for (auto& iterated_path : std::filesystem::recursive_directory_iterator(path))
        {
            std::wstring str_path = iterated_path.path().generic_wstring();
            list.emplace_back(str_path);
        }
    }

    bool FileUtility::WriteStringToFile(const std::wstring& path, const std::string& data)
    {
        std::ofstream output(path);
        if (output.is_open())
        {
            output << data;
            output.close();
            return true;
        }
        return false;
    }

    bool FileUtility::ReadAndWriteStringToFile(const std::wstring& path, const std::string& data) const
    {
        if (this->IsExist(path) == true)
        {
            std::ifstream input(path, std::ifstream::binary);
            std::string   buffer_str = "";
            if (input.is_open() == true)
            {
                std::string line;
                while (std::getline(input, line))
                {
                    buffer_str += line;
                }
                input.close();
            }
            else
            {
                return false;
            }
            buffer_str += data;
            std::ofstream output(path);
            if (output.is_open())
            {
                output << buffer_str;
                output.close();
                return true;
            }
        }
        else
        {
            std::ofstream output(path);
            if (output.is_open())
            {
                output << data;
                output.close();
                return true;
            }
        }
        return false;
    }

    bool FileUtility::WriteStringToFile(const std::wstring& path, const std::wstring& data)
    {
        std::wofstream output(path);
        if (output.is_open())
        {
            output << data;
            output.close();
            return true;
        }
        return false;
    }

    bool FileUtility::ReadAndWriteStringToFile(const std::wstring& path, const std::wstring& data) const
    {
        if (this->IsExist(path) == true)
        {
            std::wifstream input(path, std::wifstream::binary);
            std::wstring   buffer_str = L"";
            if (input.is_open() == true)
            {
                std::wstring line;
                while (std::getline(input, line))
                {
                    buffer_str += line;
                }
                input.close();
            }
            else
            {
                return false;
            }
            buffer_str += data;
            std::wofstream output(path);
            if (output.is_open())
            {
                output << buffer_str;
                output.close();
                return true;
            }
        }
        else
        {
            std::wofstream output(path);
            if (output.is_open())
            {
                output << data;
                output.close();
                return true;
            }
        }
        return false;
    }

    bool FileUtility::RemoveFile(const std::wstring& path) const
    {
        if (this->IsExist(path) == true && this->IsDirectory(path) == false)
        {
            std::filesystem::path dir(path);
            return std::filesystem::remove(dir);
        }
        return false;
    }

    bool FileUtility::RemoveAllInDirectory(const std::wstring& path) const
    {
        if (this->IsExist(path) == true && this->IsDirectory(path) == true)
        {
            std::filesystem::path dir(path);
            return (bool)remove_all(dir);
        }
        return false;
    }

    bool FileUtility::RemoveDirectory(const std::wstring& path) const
    {
        if (this->IsExist(path) == true && this->IsDirectory(path) == true)
        {
            std::filesystem::path dir(path);
            return std::filesystem::remove(dir);
        }
        return false;
    }

    bool FileUtility::IsDirectory(const std::wstring& path) const
    {
        return std::filesystem::is_directory(path);
    }

    bool FileUtility::IsExist(const std::wstring& path) const
    {
        return exists(std::filesystem::path(path));
    }

    void FileUtility::AddPathPermission(const std::wstring& path, ePermission permission) const
    {
        std::filesystem::path  dir(path);
        std::filesystem::perms p_read
                = std::filesystem::perms::owner_read
                | std::filesystem::perms::others_read
                | std::filesystem::perms::group_read;
        std::filesystem::perms p_write
                = std::filesystem::perms::owner_write
                | std::filesystem::perms::others_write
                | std::filesystem::perms::group_write;
        //switch
        switch (permission)
        {
        case ePermission::Read:
            permissions(dir, p_read, std::filesystem::perm_options::add);
            break;
        case ePermission::Write:
            permissions(dir, p_write, std::filesystem::perm_options::add);
            break;
        case ePermission::All:
            permissions(dir, std::filesystem::perms::all, std::filesystem::perm_options::add);
            break;
        default:
            break;
        }
    }

    std::wstring FileUtility::GetCurrentDirectory() const
    {
        return std::filesystem::current_path().generic_wstring();
    }

    bool FileUtility::ReadFileToFileBuffer(const std::wstring& path, std::vector<U8>& buffer)
    {
        std::wifstream file(path, std::wios::binary);
        if (file.fail())
        {
            _wperror(path.c_str());
            return false;
        }
        //seek to the end
        file.seekg(0, std::wios::end);
        //get the file size.
        auto file_size = file.tellg();
        file.seekg(0, std::wios::beg);
        //reduce the file size by any header bytes that might be present
        file_size -= file.tellg();
        buffer.resize(static_cast<size_t>(file_size));
        file.read((wchar_t*)&(buffer[0]), file_size);
        file.close();
        return true;
    }

    bool FileUtility::ReadFileToStringBuffer(const std::wstring& path, std::string& buffer)
    {
        std::wifstream file(path, std::wios::binary);
        if (file.fail())
        {
            _wperror(path.c_str());
            return false;
        }
        //seek to the end
        file.seekg(0, std::wios::end);
        //get the file size.
        auto file_size = file.tellg();
        file.seekg(0, std::wios::beg);
        //reduce the file size by any header bytes that might be present
        file_size -= file.tellg();
        buffer.resize(static_cast<size_t>(file_size));
        file.read((wchar_t*)&(buffer[0]), file_size);
        file.close();
        return true;
    }
}
