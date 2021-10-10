#pragma once
#include <string>
#include <list>
#include <unordered_map>

namespace CS460
{
    class AniMeshResource;
    class RendererCommon;
    enum class eJsonType : unsigned long long;
    class LevelManager;
    class JsonResource;
    class ShaderResource;
    class TextureResource;
    class MeshResource;
    class AudioResource;
    class TextResource;
    class UndefinedResource;
    class Resource;
    class FileUtility;

    class ResourceManager
    {
    public:
        ResourceManager();
        ~ResourceManager();

        void Initialize(FileUtility* file_utility);
        void Shutdown();

        Resource* GetResource(const std::wstring& path);
        Resource* GetResourceFileName(const std::wstring& file_name);
        void      GetResources(const std::wstring& file_name, std::vector<Resource*>& resources);

        void RemoveResource(const std::wstring& path);
        void RemoveResource(Resource* resource);
        void ClearResource();
        void BuildResource();

        std::wstring GetRootPath() const;

        //shader
        ShaderResource* GetShaderResource(const std::wstring& path);
        ShaderResource* GetShaderResourceFileName(const std::wstring& file_name);
        void            GetShaderResources(const std::wstring& file_name, std::vector<ShaderResource*>& resources);

        //texture
        TextureResource* GetTextureResource(const std::wstring& path);
        TextureResource* GetTextureResourceFileName(const std::wstring& file_name);
        void             GetTextureResources(const std::wstring& file_name, std::vector<TextureResource*>& resources);
        void             InitializeTextureResources(RendererCommon* renderer);

        //ani mesh
        AniMeshResource* GetAniMeshResource(const std::wstring& path);
        AniMeshResource* GetAniMeshResourceFileName(const std::wstring& file_name);
        void          GetAniMeshResources(const std::wstring& file_name, std::vector<AniMeshResource*>& resources);


        //mesh
        MeshResource* GetMeshResource(const std::wstring& path);
        MeshResource* GetMeshResourceFileName(const std::wstring& file_name);
        void          GetMeshResources(const std::wstring& file_name, std::vector<MeshResource*>& resources);

        //audio
        AudioResource* GetAudioResource(const std::wstring& path);
        AudioResource* GetAudioResourceFileName(const std::wstring& file_name);
        void           GetAudioResources(const std::wstring& file_name, std::vector<AudioResource*>& resources);

        //json
        JsonResource* GetJsonResource(const std::wstring& path);
        JsonResource* GetJsonResourceFileName(const std::wstring& file_name);
        void          GetJsonResources(const std::wstring& file_name, std::vector<JsonResource*>& resources);
        void          GetJsonResources(std::vector<JsonResource*>& resources);
        void          GetJsonResources(eJsonType type, std::vector<JsonResource*>& resources);
        JsonResource* CreateJsonResource(const std::wstring& path);

        //text
        TextResource* GetTextResource(const std::wstring& path);
        TextResource* GetTextResourceFileName(const std::wstring& file_name);
        void          GetTextResources(const std::wstring& file_name, std::vector<TextResource*>& resources);

        //undefined
        UndefinedResource* GetUndefinedResource(const std::wstring& path);
        UndefinedResource* GetUndefinedResourceFileName(const std::wstring& file_name);
        void               GetUndefinedResources(const std::wstring& file_name, std::vector<UndefinedResource*>& resources);

    private:
        void      QueryFilePathRecursive(const std::wstring& path, std::list<std::wstring>& file_list, std::list<std::wstring>& directory_list) const;
        Resource* AddResource(const std::wstring& path);

    private:
        friend class LevelManager;

    private:
        std::wstring            m_root_path;
        std::list<std::wstring> m_file_list;
        std::list<std::wstring> m_directory_list;

        std::unordered_map<std::wstring, UndefinedResource*> m_undefined_resource_map;
        std::unordered_map<std::wstring, ShaderResource*>    m_shader_resource_map;
        std::unordered_map<std::wstring, TextureResource*>   m_texture_resource_map;
        std::unordered_map<std::wstring, MeshResource*>      m_mesh_resource_map;
        std::unordered_map<std::wstring, AudioResource*>     m_audio_resource_map;
        std::unordered_map<std::wstring, JsonResource*>      m_json_resource_map;
        std::unordered_map<std::wstring, TextResource*>      m_text_resource_map;
        std::unordered_map<std::wstring, AniMeshResource*>   m_ani_mesh_resource_map;

        FileUtility* m_file_utility = nullptr;
    private:
    };
}
