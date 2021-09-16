// ReSharper disable CppExpressionWithoutSideEffects
#include "ResourceManager.hpp"
#include "Resource.hpp"
#include "../../System/Core/Utility/FileUtility.hpp"
#include "ResourceType/AudioResource.hpp"
#include "ResourceType/TextureResource.hpp"
#include "ResourceType/MeshResource.hpp"
#include "ResourceType/ShaderResource.hpp"
#include "ResourceType/JsonResource.hpp"
#include "ResourceType/UndefinedResource.hpp"
#include "ResourceType/TextResource.hpp"
#include "../../System/Core/Utility/CoreUtility.hpp"

namespace CS460
{
    ResourceManager::ResourceManager()
    {
    }

    ResourceManager::~ResourceManager()
    {
    }

    void ResourceManager::Initialize(FileUtility* file_utility)
    {
        m_file_utility = file_utility;
        m_root_path    = L"../../Resource";
        BuildResource();
    }

    void ResourceManager::Shutdown()
    {
        m_file_list.clear();
        m_directory_list.clear();
        ClearResource();
    }

    Resource* ResourceManager::GetResource(const std::wstring& path)
    {
        Resource* resource = nullptr;
        //find resource
        resource = resource != nullptr ? resource : GetAudioResource(path);
        resource = resource != nullptr ? resource : GetJsonResource(path);
        resource = resource != nullptr ? resource : GetMeshResource(path);
        resource = resource != nullptr ? resource : GetShaderResource(path);
        resource = resource != nullptr ? resource : GetTextureResource(path);
        resource = resource != nullptr ? resource : GetTextResource(path);
        resource = resource != nullptr ? resource : GetUndefinedResource(path);
        resource = resource != nullptr ? resource : AddResource(path);
        return resource;
    }

    Resource* ResourceManager::GetResourceFileName(const std::wstring& file_name)
    {
        Resource* resource = nullptr;
        //find resource
        resource = resource != nullptr ? resource : GetAudioResourceFileName(file_name);
        resource = resource != nullptr ? resource : GetJsonResourceFileName(file_name);
        resource = resource != nullptr ? resource : GetMeshResourceFileName(file_name);
        resource = resource != nullptr ? resource : GetShaderResourceFileName(file_name);
        resource = resource != nullptr ? resource : GetTextureResourceFileName(file_name);
        resource = resource != nullptr ? resource : GetTextResourceFileName(file_name);
        resource = resource != nullptr ? resource : GetUndefinedResourceFileName(file_name);
        return resource;
    }

    void ResourceManager::GetResources(const std::wstring& file_name, std::vector<Resource*>& resources)
    {
        for (auto& resource : m_audio_resource_map)
        {
            auto found = resource.second;
            if (found->WFileName() == file_name)
            {
                resources.push_back(found);
            }
            else if (found->WFileName() + found->WFileType() == file_name)
            {
                resources.push_back(found);
            }
        }
        for (auto& resource : m_json_resource_map)
        {
            auto found = resource.second;
            if (found->WFileName() == file_name)
            {
                resources.push_back(found);
            }
            else if (found->WFileName() + found->WFileType() == file_name)
            {
                resources.push_back(found);
            }
        }
        for (auto& resource : m_mesh_resource_map)
        {
            auto found = resource.second;
            if (found->WFileName() == file_name)
            {
                resources.push_back(found);
            }
            else if (found->WFileName() + found->WFileType() == file_name)
            {
                resources.push_back(found);
            }
        }
        for (auto& resource : m_shader_resource_map)
        {
            auto found = resource.second;
            if (found->WFileName() == file_name)
            {
                resources.push_back(found);
            }
            else if (found->WFileName() + found->WFileType() == file_name)
            {
                resources.push_back(found);
            }
        }
        for (auto& resource : m_texture_resource_map)
        {
            auto found = resource.second;
            if (found->WFileName() == file_name)
            {
                resources.push_back(found);
            }
            else if (found->WFileName() + found->WFileType() == file_name)
            {
                resources.push_back(found);
            }
        }
        for (auto& resource : m_text_resource_map)
        {
            auto found = resource.second;
            if (found->WFileName() == file_name)
            {
                resources.push_back(found);
            }
            else if (found->WFileName() + found->WFileType() == file_name)
            {
                resources.push_back(found);
            }
        }
        for (auto& resource : m_undefined_resource_map)
        {
            auto found = resource.second;
            if (found->WFileName() == file_name)
            {
                resources.push_back(found);
            }
            else if (found->WFileName() + found->WFileType() == file_name)
            {
                resources.push_back(found);
            }
        }
    }

    void ResourceManager::RemoveResource(const std::wstring& path)
    {
        auto found_in_resource = m_undefined_resource_map.find(path);
        if (found_in_resource != m_undefined_resource_map.end())
        {
            if (found_in_resource->second->IsLoaded() == true &&
                found_in_resource->second->IsUnloaded() == true)
            {
                if (found_in_resource->second != nullptr)
                {
                    delete found_in_resource->second;
                    found_in_resource->second = nullptr;
                }
                m_undefined_resource_map.erase(found_in_resource);
            }
        }
        auto found_in_json = m_json_resource_map.find(path);
        if (found_in_json != m_json_resource_map.end())
        {
            if (found_in_json->second->IsLoaded() == true &&
                found_in_json->second->IsUnloaded() == true)
            {
                if (found_in_json->second != nullptr)
                {
                    delete found_in_json->second;
                    found_in_json->second = nullptr;
                }
                m_json_resource_map.erase(found_in_json);
            }
        }
        auto found_in_audio = m_audio_resource_map.find(path);
        if (found_in_audio != m_audio_resource_map.end())
        {
            if (found_in_audio->second->IsLoaded() == true &&
                found_in_audio->second->IsUnloaded() == true)
            {
                if (found_in_audio->second != nullptr)
                {
                    delete found_in_audio->second;
                    found_in_audio->second = nullptr;
                }
                m_audio_resource_map.erase(found_in_audio);
            }
        }
        auto found_in_texture = m_texture_resource_map.find(path);
        if (found_in_texture != m_texture_resource_map.end())
        {
            if (found_in_texture->second->IsLoaded() == true &&
                found_in_texture->second->IsUnloaded() == true)
            {
                if (found_in_texture->second != nullptr)
                {
                    delete found_in_texture->second;
                    found_in_texture->second = nullptr;
                }
                m_texture_resource_map.erase(found_in_texture);
            }
        }
        auto found_in_text = m_text_resource_map.find(path);
        if (found_in_text != m_text_resource_map.end())
        {
            if (found_in_text->second->IsLoaded() == true &&
                found_in_text->second->IsUnloaded() == true)
            {
                if (found_in_text->second != nullptr)
                {
                    delete found_in_text->second;
                    found_in_text->second = nullptr;
                }
                m_text_resource_map.erase(found_in_text);
            }
        }
        auto found_in_mesh = m_mesh_resource_map.find(path);
        if (found_in_mesh != m_mesh_resource_map.end())
        {
            if (found_in_mesh->second->IsLoaded() == true &&
                found_in_mesh->second->IsUnloaded() == true)
            {
                if (found_in_mesh->second != nullptr)
                {
                    delete found_in_mesh->second;
                    found_in_mesh->second = nullptr;
                }
                m_mesh_resource_map.erase(found_in_mesh);
            }
        }
        auto found_in_shader = m_shader_resource_map.find(path);
        if (found_in_shader != m_shader_resource_map.end())
        {
            if (found_in_shader->second->IsLoaded() == true &&
                found_in_shader->second->IsUnloaded() == true)
            {
                if (found_in_shader->second != nullptr)
                {
                    delete found_in_shader->second;
                    found_in_shader->second = nullptr;
                }
                m_shader_resource_map.erase(found_in_shader);
            }
        }
    }

    void ResourceManager::RemoveResource(Resource* resource)
    {
        RemoveResource(resource->WFilePath());
    }

    void ResourceManager::ClearResource()
    {
        m_file_utility->RemoveFile(L"Resource Manager Report.txt");
        for (auto& resource : m_shader_resource_map)
        {
            auto found = resource.second;
            if (found != nullptr)
            {
                found->Shutdown();
                if (found->IsLoaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (found->IsUnloaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete found;
                found = nullptr;
            }
        }
        m_shader_resource_map.clear();
        for (auto& resource : m_mesh_resource_map)
        {
            auto found = resource.second;
            if (found != nullptr)
            {
                found->Shutdown();
                if (found->IsLoaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (found->IsUnloaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete found;
                found = nullptr;
            }
        }
        m_mesh_resource_map.clear();
        for (auto& resource : m_texture_resource_map)
        {
            auto found = resource.second;
            if (found != nullptr)
            {
                found->Shutdown();
                if (found->IsLoaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (found->IsUnloaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete found;
                found = nullptr;
            }
        }
        m_texture_resource_map.clear();
        for (auto& resource : m_audio_resource_map)
        {
            auto found = resource.second;
            if (found != nullptr)
            {
                found->Shutdown();
                if (found->IsLoaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (found->IsUnloaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete found;
                found = nullptr;
            }
        }
        m_audio_resource_map.clear();
        for (auto& resource : m_json_resource_map)
        {
            auto found = resource.second;
            if (found != nullptr)
            {
                found->Shutdown();
                if (found->IsLoaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (found->IsUnloaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete found;
                found = nullptr;
            }
        }
        m_json_resource_map.clear();
        //remove text
        for (auto& resource : m_text_resource_map)
        {
            auto found = resource.second;
            if (found != nullptr)
            {
                found->Shutdown();
                if (found->IsLoaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (found->IsUnloaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete found;
                found = nullptr;
            }
        }
        m_text_resource_map.clear();
        //remove remains
        for (auto& resource : m_undefined_resource_map)
        {
            auto found = resource.second;
            if (found != nullptr)
            {
                found->Shutdown();
                if (found->IsLoaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't loaded. this file may not used in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                if (found->IsUnloaded() == false)
                {
                    std::wstring result = found->WFileName() + found->WFileType();
                    result += L" file didn't unloaded. this file may not released in engine. \n";
                    m_file_utility->ReadAndWriteStringToFile(L"Resource Manager Report.txt", result);
                }
                delete found;
                found = nullptr;
            }
        }
        m_undefined_resource_map.clear();
    }

    void ResourceManager::BuildResource()
    {
        m_file_list.clear();
        m_directory_list.clear();
        QueryFilePathRecursive(m_root_path, m_file_list, m_directory_list);
        for (auto& file : m_file_list)
        {
            AddResource(file);
        }

        TextureResource* default_texture = new TextureResource(L"DefaultTexture");
        m_texture_resource_map.emplace(L"DefaultTexture", default_texture);
    }

    std::wstring ResourceManager::GetRootPath() const
    {
        return m_root_path;
    }

    ShaderResource* ResourceManager::GetShaderResource(const std::wstring& path)
    {
        auto found = m_shader_resource_map.find(path);
        if (found != m_shader_resource_map.end())
        {
            return found->second;
        }
        return nullptr;
    }

    ShaderResource* ResourceManager::GetShaderResourceFileName(const std::wstring& file_name)
    {
        for (auto& shader : m_shader_resource_map)
        {
            auto resource = shader.second;
            if (resource->WFileName() + resource->WFileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetShaderResources(const std::wstring& file_name, std::vector<ShaderResource*>& resources)
    {
        for (auto& shader : m_shader_resource_map)
        {
            auto resource = shader.second;
            if (resource->WFileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->WFileName() + resource->WFileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    TextureResource* ResourceManager::GetTextureResource(const std::wstring& path)
    {
        auto found = m_texture_resource_map.find(path);
        if (found != m_texture_resource_map.end())
        {
            return found->second;
        }
        return nullptr;
    }

    TextureResource* ResourceManager::GetTextureResourceFileName(const std::wstring& file_name)
    {
        for (auto& texture : m_texture_resource_map)
        {
            auto resource = texture.second;
            if (resource->WFileName() + resource->WFileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetTextureResources(const std::wstring& file_name, std::vector<TextureResource*>& resources)
    {
        for (auto& texture : m_texture_resource_map)
        {
            auto resource = texture.second;
            if (resource->WFileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->WFileName() + resource->WFileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    void ResourceManager::InitializeTextureResources(RendererCommon* renderer)
    {
        for (auto& texture : m_texture_resource_map)
        {
            auto resource = texture.second;
            resource->SetRenderer(renderer);
        }
    }

    MeshResource* ResourceManager::GetMeshResource(const std::wstring& path)
    {
        auto found = m_mesh_resource_map.find(path);
        if (found != m_mesh_resource_map.end())
        {
            return found->second;
        }
        return nullptr;
    }

    MeshResource* ResourceManager::GetMeshResourceFileName(const std::wstring& file_name)
    {
        for (auto& mesh : m_mesh_resource_map)
        {
            auto resource = mesh.second;
            if (resource->WFileName() + resource->WFileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetMeshResources(const std::wstring& file_name, std::vector<MeshResource*>& resources)
    {
        for (auto& mesh : m_mesh_resource_map)
        {
            auto resource = mesh.second;
            if (resource->WFileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->WFileName() + resource->WFileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    AudioResource* ResourceManager::GetAudioResource(const std::wstring& path)
    {
        auto found = m_audio_resource_map.find(path);
        if (found != m_audio_resource_map.end())
        {
            return found->second;
        }
        return nullptr;
    }

    AudioResource* ResourceManager::GetAudioResourceFileName(const std::wstring& file_name)
    {
        for (auto& audio : m_audio_resource_map)
        {
            auto resource = audio.second;
            if (resource->WFileName() + resource->WFileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetAudioResources(const std::wstring& file_name, std::vector<AudioResource*>& resources)
    {
        for (auto& audio : m_audio_resource_map)
        {
            auto resource = audio.second;
            if (resource->WFileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->WFileName() + resource->WFileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    JsonResource* ResourceManager::GetJsonResource(const std::wstring& path)
    {
        auto found = m_json_resource_map.find(path);
        if (found != m_json_resource_map.end())
        {
            return found->second;
        }
        return nullptr;
    }

    JsonResource* ResourceManager::GetJsonResourceFileName(const std::wstring& file_name)
    {
        for (auto& json : m_json_resource_map)
        {
            auto resource = json.second;
            if (resource->WFileName() + resource->WFileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetJsonResources(const std::wstring& file_name, std::vector<JsonResource*>& resources)
    {
        for (auto& json : m_json_resource_map)
        {
            auto resource = json.second;
            if (resource->WFileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->WFileName() + resource->WFileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    void ResourceManager::GetJsonResources(std::vector<JsonResource*>& resources)
    {
        for (auto& json : m_json_resource_map)
        {
            resources.push_back(json.second);
        }
    }

    void ResourceManager::GetJsonResources(eJsonType type, std::vector<JsonResource*>& resources)
    {
        for (auto& json : m_json_resource_map)
        {
            auto resource = json.second;
            if (resource->GetType() == type)
            {
                resources.push_back(resource);
            }
        }
    }

    JsonResource* ResourceManager::CreateJsonResource(const std::wstring& path)
    {
        JsonResource* resource       = new JsonResource(path);
        resource->m_file_name_w      = m_file_utility->GetFileNameFromPath(path);
        resource->m_file_type_w      = m_file_utility->GetFileTypeFromPath(path);
        resource->m_resource_manager = this;
        m_json_resource_map.emplace(path, resource);
        return resource;
    }

    TextResource* ResourceManager::GetTextResource(const std::wstring& path)
    {
        auto found = m_text_resource_map.find(path);
        if (found != m_text_resource_map.end())
        {
            return found->second;
        }
        return nullptr;
    }

    TextResource* ResourceManager::GetTextResourceFileName(const std::wstring& file_name)
    {
        for (auto& json : m_text_resource_map)
        {
            auto resource = json.second;
            if (resource->WFileName() + resource->WFileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetTextResources(const std::wstring& file_name, std::vector<TextResource*>& resources)
    {
        for (auto& json : m_text_resource_map)
        {
            auto resource = json.second;
            if (resource->WFileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->WFileName() + resource->WFileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    UndefinedResource* ResourceManager::GetUndefinedResource(const std::wstring& path)
    {
        auto found = m_undefined_resource_map.find(path);
        if (found != m_undefined_resource_map.end())
        {
            return found->second;
        }
        return nullptr;
    }

    UndefinedResource* ResourceManager::GetUndefinedResourceFileName(const std::wstring& file_name)
    {
        for (auto& other : m_undefined_resource_map)
        {
            auto resource = other.second;
            if (resource->WFileName() + resource->WFileType() == file_name)
            {
                return resource;
            }
        }
        return nullptr;
    }

    void ResourceManager::GetUndefinedResources(const std::wstring& file_name, std::vector<UndefinedResource*>& resources)
    {
        for (auto& other : m_undefined_resource_map)
        {
            auto resource = other.second;
            if (resource->WFileName() == file_name)
            {
                resources.push_back(resource);
            }
            else if (resource->WFileName() + resource->WFileType() == file_name)
            {
                resources.push_back(resource);
            }
        }
    }

    void ResourceManager::QueryFilePathRecursive(const std::wstring& path, std::list<std::wstring>& file_list, std::list<std::wstring>& directory_list) const
    {
        std::list<std::wstring> paths;
        m_file_utility->GetFileListsInPath(path, paths);
        for (auto file_path : paths)
        {
            if (m_file_utility->IsDirectory(file_path))
            {
                directory_list.push_back(file_path);
                QueryFilePathRecursive(file_path, file_list, directory_list);
            }
            else
            {
                file_list.push_back(file_path);
            }
        }
    }

    Resource* ResourceManager::AddResource(const std::wstring& path)
    {
        Resource*    resource;
        std::wstring type = m_file_utility->GetFileTypeFromPath(path);
        std::wstring name = m_file_utility->GetFileNameFromPath(path);
        if (type == L".fx" || type == L".hlsl" || type == L".shader")
        {
            resource = new ShaderResource(path);
            ((ShaderResource*)resource)->SetStage(eShaderStage::IntegratedShader);
            m_shader_resource_map.emplace(path, (ShaderResource*)resource);
        }
        else if( type == L".ps")
        {
            resource = new ShaderResource(path);
            ((ShaderResource*)resource)->SetStage(eShaderStage::PixelShader);
            m_shader_resource_map.emplace(path, (ShaderResource*)resource);
        }
        else if( type == L".vs")
        {
            resource = new ShaderResource(path);
            ((ShaderResource*)resource)->SetStage(eShaderStage::VertexShader);
            m_shader_resource_map.emplace(path, (ShaderResource*)resource);
        }
        else if (
            type == L".dds" || type == L".DDS" ||
            type == L".png" || type == L".PNG" ||
            type == L".jpeg" || type == L".JPEG" ||
            type == L".jpg" || type == L".JPG" ||
            type == L".bmp" || type == L".BMP" ||
            type == L".tiff" || type == L".TIFF" ||
            type == L".gif" || type == L".GIF")
        {
            resource = new TextureResource(path);
            m_texture_resource_map.emplace(path, (TextureResource*)resource);
        }
        else if (type == L".obj")
        {
            resource = new MeshResource(path);
            m_mesh_resource_map.emplace(path, (MeshResource*)resource);
        }
        else if (type == L".wav")
        {
            resource = new AudioResource(path);
            m_audio_resource_map.emplace(path, (AudioResource*)resource);
        }
        else if (type == L".json")
        {
            resource = new JsonResource(path);
            m_json_resource_map.emplace(path, (JsonResource*)resource);
        }
        else if (type == L".txt")
        {
            resource = new TextResource(path);
            m_text_resource_map.emplace(path, (TextResource*)resource);
        }
        else
        {
            resource = new UndefinedResource(path);
            m_undefined_resource_map.emplace(path, (UndefinedResource*)resource);
        }
        resource->m_file_name_w      = name;
        resource->m_file_type_w      = type;
        resource->m_file_path_m      = ToString(path);
        resource->m_file_name_m      = ToString(name);
        resource->m_file_type_m      = ToString(type);
        resource->m_resource_manager = this;
        resource->Initialize();
        return resource;
    }
}
