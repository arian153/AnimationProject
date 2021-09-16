#pragma once
#include <string>
#include <list>
#include "CoreDef.hpp"
#include <vector>

namespace CS460
{
    enum class ePermission
    {
        Read
      , Write
      , All
    };

    class FileUtility
    {
    public:
        FileUtility();
        ~FileUtility();

        std::wstring GetFileNameFromPath(const std::wstring& path);
        std::wstring GetFileTypeFromPath(const std::wstring& path);
        std::wstring GetParentPathFromPath(const std::wstring& path);

        void GetFileListsInPath(const std::wstring& path, std::list<std::wstring>& list) const;
        void GetFileListsInPathRecursive(const std::wstring& path, std::list<std::wstring>& list);

        bool WriteStringToFile(const std::wstring& path, const std::string& data);
        bool ReadAndWriteStringToFile(const std::wstring& path, const std::string& data) const;
        bool WriteStringToFile(const std::wstring& path, const std::wstring& data);
        bool ReadAndWriteStringToFile(const std::wstring& path, const std::wstring& data) const;
        bool RemoveFile(const std::wstring& path) const;
        bool RemoveAllInDirectory(const std::wstring& path) const;
        bool RemoveDirectory(const std::wstring& path) const;

        bool IsDirectory(const std::wstring& path) const;
        bool IsExist(const std::wstring& path) const;

        void AddPathPermission(const std::wstring& path, ePermission permission) const;

        std::wstring GetCurrentDirectory() const;

        bool ReadFileToFileBuffer(const std::wstring& path, std::vector<U8>& buffer);
        bool ReadFileToStringBuffer(const std::wstring& path, std::string& buffer);
    private:
       
    };
}
