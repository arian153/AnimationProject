#pragma once
#include "../Resource.hpp"
#include "../../../System/Math/Math.hpp"

namespace Json
{
    class CharReader;
    class Value;
}

namespace CS460
{
    class ObjectFactory;
    class Object;
    struct ApplicationSetting;
    class Space;
    class Level;

    enum class eJsonType : size_t
    {
        None,
        Setting,
        Level,
        Space,
        Archetype,
        MeshData,
        ParticleData
    };

    class JsonResource final : public Resource
    {
    public:
        explicit JsonResource(const std::wstring& path);
        ~JsonResource();

        void Initialize() override;
        void Shutdown() override;

        bool      IsSetting() const;
        bool      IsLevel() const;
        bool      IsSpace() const;
        bool      IsArchetype() const;
        bool      IsMeshData() const;
        bool      IsParticleData() const;
        eJsonType GetType() const;
        bool      LoadJsonType();

        bool  WasOpen() const;
        bool  IsOpen() const;
        bool& IsOpen();
        bool  IsModified() const;
        bool& IsModified();
        bool  IsClose() const;
        bool& IsClose();

        void UpdateOpenState();

    public:
        static bool HasMember(const Json::Value& data, const std::string& find);
        static bool IsVector2(const Json::Value& data);
        static bool IsVector3(const Json::Value& data);
        static bool IsVector4(const Json::Value& data);
        static bool IsMatrix22(const Json::Value& data);
        static bool IsMatrix33(const Json::Value& data);
        static bool IsMatrix44(const Json::Value& data);
        static bool IsQuaternion(const Json::Value& data);
        static bool IsColor(const Json::Value& data);

        static Vector2    AsVector2(const Json::Value& data);
        static Vector3    AsVector3(const Json::Value& data);
        static Vector4    AsVector4(const Json::Value& data);
        static Matrix22   AsMatrix22(const Json::Value& data);
        static Matrix33   AsMatrix33(const Json::Value& data);
        static Matrix44   AsMatrix44(const Json::Value& data);
        static Quaternion AsQuaternionRIJK(const Json::Value& data);
        static Quaternion AsQuaternionXYZW(const Json::Value& data);
        static Color      AsColor(const Json::Value& data);

    private:
        bool LoadSetting(ApplicationSetting& app_setting) const;
        bool LoadData(Level* level) const;
        bool LoadData(Space* space) const;
        bool LoadSpaceFlag(Space* space) const;

        //Create Empty Json Data
        void CreateLevelData(const std::wstring& space_path) const;
        void CreateSpaceData() const;

    private:
        friend class Space;
        friend class Level;
        friend class LevelManager;
        friend class Application;
        friend class ObjectFactory;

    private:
        eJsonType         m_json_type   = eJsonType::None;
        Json::CharReader* m_reader      = nullptr;
        Json::Value*      m_root_data   = nullptr;
        bool              m_b_load_type = true;
        //editor flag
        bool m_b_curr_open = false;
        bool m_b_prev_open = false;
        bool m_b_modified  = false;
        bool m_b_close     = false;
    };
}
