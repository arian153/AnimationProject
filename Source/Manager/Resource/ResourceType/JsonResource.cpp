#include "JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include <fstream>
#include "../../Level/Level.hpp"
#include "../ResourceManager.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../../System/Core/OS-API/ApplicationSetting.hpp"
#include "../../Object/Object.hpp"
#include "../../Object/ObjectManager.hpp"
#include "../../Object/ObjectFactory.hpp"
#include "../../../System/Graphics/DataType/Color.hpp"
#include "../../../System/Physics/Dynamics/World.hpp"

namespace CS460
{
    JsonResource::JsonResource(const std::wstring& path)
        : Resource(path)
    {
        m_type = eResourceType::Json;
    }

    JsonResource::~JsonResource()
    {
    }

    void JsonResource::Initialize()
    {
        Json::CharReaderBuilder builder;
        m_reader    = builder.newCharReader();
        m_root_data = new Json::Value();
        if (m_b_load_type)
        {
            LoadJsonType();
        }
        m_b_loaded = true;
    }

    void JsonResource::Shutdown()
    {
        if (m_reader != nullptr)
        {
            delete m_reader;
            m_reader = nullptr;
        }
        if (m_root_data != nullptr)
        {
            delete m_root_data;
            m_root_data = nullptr;
        }
        m_b_unloaded = true;
    }

    bool JsonResource::IsSetting() const
    {
        return m_json_type == eJsonType::Setting;
    }

    bool JsonResource::IsLevel() const
    {
        return m_json_type == eJsonType::Level;
    }

    bool JsonResource::IsSpace() const
    {
        return m_json_type == eJsonType::Space;
    }

    bool JsonResource::IsArchetype() const
    {
        return m_json_type == eJsonType::Archetype;
    }

    bool JsonResource::IsMeshData() const
    {
        return m_json_type == eJsonType::MeshData;
    }

    bool JsonResource::IsParticleData() const
    {
        return m_json_type == eJsonType::ParticleData;
    }

    eJsonType JsonResource::GetType() const
    {
        return m_json_type;
    }

    bool JsonResource::LoadJsonType()
    {
        std::ifstream file(m_file_path_w, std::ifstream::binary);
        std::string   doc;
        std::getline(file, doc, static_cast<char>(EOF));
        bool b_parsing_successful = m_reader->parse(doc.data(), doc.data() + doc.size(), m_root_data, nullptr);
        if (b_parsing_successful)
        {
            if (HasMember(*m_root_data, "Type") && (*m_root_data)["Type"].isString())
            {
                std::string json_type = (*m_root_data)["Type"].asString();
                if (json_type == "Setting")
                {
                    m_json_type = eJsonType::Setting;
                }
                else if (json_type == "Level")
                {
                    m_json_type = eJsonType::Level;
                }
                else if (json_type == "Space")
                {
                    m_json_type = eJsonType::Space;
                }
                else if (json_type == "Archetype")
                {
                    m_json_type = eJsonType::Archetype;
                }
                else if (json_type == "MeshData")
                {
                    m_json_type = eJsonType::MeshData;
                }
                else if (json_type == "ParticleData")
                {
                    m_json_type = eJsonType::ParticleData;
                }
                else
                {
                    m_json_type = eJsonType::None;
                }
                return true;
            }
        }
        return false;
    }

    bool JsonResource::WasOpen() const
    {
        return m_b_prev_open;
    }

    bool JsonResource::IsOpen() const
    {
        return m_b_curr_open;
    }

    bool& JsonResource::IsOpen()
    {
        return m_b_curr_open;
    }

    bool JsonResource::IsModified() const
    {
        return m_b_modified;
    }

    bool& JsonResource::IsModified()
    {
        return m_b_modified;
    }

    bool JsonResource::IsClose() const
    {
        return m_b_close;
    }

    bool& JsonResource::IsClose()
    {
        return m_b_close;
    }

    void JsonResource::UpdateOpenState()
    {
        m_b_prev_open = m_b_curr_open;
    }

    bool JsonResource::HasMember(const Json::Value& data, const std::string& find)
    {
        return !data[find].isNull();
    }

    bool JsonResource::IsVector2(const Json::Value& data)
    {
        return data.isArray() &&
                data.size() == 2 &&
                data[0].isNumeric() &&
                data[1].isNumeric();
    }

    bool JsonResource::IsVector3(const Json::Value& data)
    {
        return data.isArray() &&
                data.size() == 3 &&
                data[0].isNumeric() &&
                data[1].isNumeric() &&
                data[2].isNumeric();
    }

    bool JsonResource::IsVector4(const Json::Value& data)
    {
        return data.isArray() &&
                data.size() == 4 &&
                data[0].isNumeric() &&
                data[1].isNumeric() &&
                data[2].isNumeric() &&
                data[3].isNumeric();
    }

    bool JsonResource::IsMatrix22(const Json::Value& data)
    {
        return data.isArray() &&
                data.size() == 2 &&
                IsVector2(data[0]) &&
                IsVector2(data[1]);
    }

    bool JsonResource::IsMatrix33(const Json::Value& data)
    {
        return data.isArray() &&
                data.size() == 3 &&
                IsVector3(data[0]) &&
                IsVector3(data[1]) &&
                IsVector3(data[2]);
    }

    bool JsonResource::IsMatrix44(const Json::Value& data)
    {
        return data.isArray() &&
                data.size() == 4 &&
                IsVector4(data[0]) &&
                IsVector4(data[1]) &&
                IsVector4(data[2]) &&
                IsVector4(data[3]);
    }

    bool JsonResource::IsQuaternion(const Json::Value& data)
    {
        return data.isArray() &&
                data.size() == 4 &&
                data[0].isNumeric() &&
                data[1].isNumeric() &&
                data[2].isNumeric() &&
                data[3].isNumeric();
    }

    bool JsonResource::IsColor(const Json::Value& data)
    {
        return data.isArray() &&
                data.size() == 4 &&
                data[0].isNumeric() &&
                data[1].isNumeric() &&
                data[2].isNumeric() &&
                data[3].isNumeric();
    }

    Vector2 JsonResource::AsVector2(const Json::Value& data)
    {
        return Vector2(data[0].asFloat(), data[1].asFloat());
    }

    Vector3 JsonResource::AsVector3(const Json::Value& data)
    {
        return Vector3(data[0].asFloat(), data[1].asFloat(), data[2].asFloat());
    }

    Vector4 JsonResource::AsVector4(const Json::Value& data)
    {
        return Vector4(data[0].asFloat(), data[1].asFloat(), data[2].asFloat(), data[3].asFloat());
    }

    Matrix22 JsonResource::AsMatrix22(const Json::Value& data)
    {
        Matrix22 result;
        result.SetRows(AsVector2(data[0]), AsVector2(data[1]));
        return result;
    }

    Matrix33 JsonResource::AsMatrix33(const Json::Value& data)
    {
        Matrix33 result;
        result.SetRows(AsVector3(data[0]), AsVector3(data[1]), AsVector3(data[2]));
        return result;
    }

    Matrix44 JsonResource::AsMatrix44(const Json::Value& data)
    {
        Matrix44 result;
        result.SetRows(AsVector4(data[0]), AsVector4(data[1]), AsVector4(data[2]), AsVector4(data[3]));
        return result;
    }

    Quaternion JsonResource::AsQuaternionRIJK(const Json::Value& data)
    {
        return Quaternion(data[0].asFloat(), data[1].asFloat(), data[2].asFloat(), data[3].asFloat());
    }

    Quaternion JsonResource::AsQuaternionXYZW(const Json::Value& data)
    {
        return Quaternion(data[3].asFloat(), data[0].asFloat(), data[1].asFloat(), data[2].asFloat());
    }

    Color JsonResource::AsColor(const Json::Value& data)
    {
        return Color(data[0].asFloat(), data[1].asFloat(), data[2].asFloat(), data[3].asFloat());
    }

    bool JsonResource::LoadSetting(ApplicationSetting& app_setting) const
    {
        if (HasMember(*m_root_data, "Settings"))
        {
            Json::Value setting = (*m_root_data)["Settings"];
            if (HasMember(setting, "Confine Cursor") && setting["Confine Cursor"].isBool())
            {
                app_setting.b_confine_cursor = setting["Confine Cursor"].asBool();
            }
            if (HasMember(setting, "Show Cursor") && setting["Show Cursor"].isBool())
            {
                app_setting.b_show_cursor = setting["Show Cursor"].asBool();
            }
            if (HasMember(setting, "Screen Resolution") && setting["Screen Resolution"].isArray())
            {
                app_setting.screen_width  = setting["Screen Resolution"][0].asInt();
                app_setting.screen_height = setting["Screen Resolution"][1].asInt();
            }
            if (HasMember(setting, "Screen Scale") && setting["Screen Scale"].isDouble())
            {
                app_setting.screen_scale = setting["Screen Scale"].asFloat();
            }
            if (HasMember(setting, "V-Sync") && setting["V-Sync"].isBool())
            {
                app_setting.b_v_sync = setting["V-Sync"].asBool();
            }
            if (HasMember(setting, "Window Mode") && setting["Window Mode"].isString())
            {
                std::string type = setting["Window Mode"].asString();
                if (type == "Windowed")
                {
                    app_setting.window_mode = eWindowMode::Windowed;
                }
                else if (type == "Fullscreen")
                {
                    app_setting.window_mode = eWindowMode::Fullscreen;
                }
                else if (type == "Borderless")
                {
                    app_setting.window_mode = eWindowMode::Borderless;
                }
            }
            if (HasMember(setting, "Window Caption") && setting["Window Caption"].isString())
            {
                app_setting.caption = setting["Window Caption"].asString();
            }
            if (HasMember(setting, "Initial Level") && setting["Initial Level"].isString())
            {
                app_setting.initial_level = setting["Initial Level"].asString();
            }
            return true;
        }
        return false;
    }

    bool JsonResource::LoadData(Level* level) const
    {
        if (HasMember(*m_root_data, "Spaces") && (*m_root_data)["Spaces"].isArray())
        {
            for (auto it = (*m_root_data)["Spaces"].begin(); it != (*m_root_data)["Spaces"].end(); ++it)
            {
                if (it->isString())
                {
                    std::wstring path           = m_resource_manager->GetRootPath() + L"/" + ToWString(it->asString());
                    auto         space_resource = m_resource_manager->GetJsonResource(path);
                    level->AddSpaceResource(space_resource);
                }
            }
        }
        return true;
    }

    bool JsonResource::LoadData(Space* space) const
    {
        if (HasMember(*m_root_data, "Settings"))
        {
            Json::Value setting = (*m_root_data)["Settings"];
            if (HasMember(setting, "ProjectionMatrix"))
            {
                std::string type = setting["ProjectionMatrix"].asString();
                if (type == "Perspective")
                {
                    space->GetScene()->SetProjectionType(eProjectionType::Perspective);
                }
                else if (type == "OrthoGraphic")
                {
                    space->GetScene()->SetProjectionType(eProjectionType::OrthoGraphic);
                }
            }
            if (HasMember(setting, "World"))
            {
                space->GetWorld()->Load(setting["World"]);
            }
        }
        if (HasMember(*m_root_data, "Objects") && (*m_root_data)["Objects"].isArray())
        {
            for (auto it = (*m_root_data)["Objects"].begin(); it != (*m_root_data)["Objects"].end(); ++it)
            {
                //Load Object
                if (HasMember(*it, "Name") && (*it)["Name"].isString())
                {
                    std::string name = (*it)["Name"].asString();
                    Object*     created_object;
                    //Check Archetype
                    if (HasMember(*it, "Archetype"))
                    {
                        size_t archetype_id = space->m_object_manager->m_object_factory->InvalidID();
                        if ((*it)["Archetype"].isString())
                        {
                            auto path     = (*it)["Archetype"].asString();
                            auto resource = m_resource_manager->GetJsonResource(ToWString(path));
                            archetype_id  = space->m_object_manager->m_object_factory->GetArchetypeID(resource);
                        }
                        else if ((*it)["Archetype"].isUInt())
                        {
                            archetype_id = (*it)["Archetype"].asUInt64();
                        }
                        if (space->m_object_manager->m_object_factory->ValidateID(archetype_id))
                        {
                            created_object = space->m_object_manager->AddObject(name, archetype_id);
                        }
                        else
                        {
                            created_object = space->GetObjectManager()->AddObject(name);
                        }
                    }
                    else
                    {
                        created_object = space->GetObjectManager()->AddObject(name);
                    }
                    created_object->Load(*it);
                }
            }
        }
        return true;
    }

    bool JsonResource::LoadSpaceFlag(Space* space) const
    {
        if (HasMember(*m_root_data, "Create Flag") && (*m_root_data)["Create Flag"].isArray())
        {
            for (auto it = (*m_root_data)["Create Flag"].begin(); it != (*m_root_data)["Create Flag"].end(); ++it)
            {
                if (it->isString())
                {
                    std::string flag = it->asString();
                    if (flag == "ComponentManager")
                    {
                        space->m_creation_flag |= eSubsystemFlag::ComponentManager;
                    }
                    else if (flag == "ObjectManager")
                    {
                        space->m_creation_flag |= eSubsystemFlag::ObjectManager;
                    }
                    else if (flag == "Scene")
                    {
                        space->m_creation_flag |= eSubsystemFlag::Scene;
                    }
                    else if (flag == "World")
                    {
                        space->m_creation_flag |= eSubsystemFlag::World;
                    }
                    else if (flag == "Logic")
                    {
                        space->m_creation_flag |= eSubsystemFlag::Logic;
                    }
                }
            }
        }
        if (HasMember(*m_root_data, "Update Flag") && (*m_root_data)["Update Flag"].isArray())
        {
            for (auto it = (*m_root_data)["Update Flag"].begin(); it != (*m_root_data)["Update Flag"].end(); ++it)
            {
                if (it->isString())
                {
                    std::string flag = it->asString();
                    if (flag == "ComponentManager")
                    {
                        space->m_update_flag |= eSubsystemFlag::ComponentManager;
                    }
                    else if (flag == "ObjectManager")
                    {
                        space->m_update_flag |= eSubsystemFlag::ObjectManager;
                    }
                    else if (flag == "Scene")
                    {
                        space->m_update_flag |= eSubsystemFlag::Scene;
                    }
                    else if (flag == "World")
                    {
                        space->m_update_flag |= eSubsystemFlag::World;
                    }
                    else if (flag == "Logic")
                    {
                        space->m_update_flag |= eSubsystemFlag::Logic;
                    }
                }
            }
        }
        if (HasMember(*m_root_data, "FixedUpdate Flag") && (*m_root_data)["FixedUpdate Flag"].isArray())
        {
            for (auto it = (*m_root_data)["FixedUpdate Flag"].begin(); it != (*m_root_data)["FixedUpdate Flag"].end(); ++it)
            {
                if (it->isString())
                {
                    std::string flag = it->asString();
                    if (flag == "ComponentManager")
                    {
                        space->m_fixed_update_flag |= eSubsystemFlag::ComponentManager;
                    }
                    else if (flag == "ObjectManager")
                    {
                        space->m_fixed_update_flag |= eSubsystemFlag::ObjectManager;
                    }
                    else if (flag == "Scene")
                    {
                        space->m_fixed_update_flag |= eSubsystemFlag::Scene;
                    }
                    else if (flag == "World")
                    {
                        space->m_fixed_update_flag |= eSubsystemFlag::World;
                    }
                    else if (flag == "Logic")
                    {
                        space->m_fixed_update_flag |= eSubsystemFlag::Logic;
                    }
                }
            }
        }
        return true;
    }

    void JsonResource::CreateLevelData(const std::wstring& space_path) const
    {
        (*m_root_data)["Type"].append("Level");
        (*m_root_data)["Spaces"].append(ToString(space_path));
    }

    void JsonResource::CreateSpaceData() const
    {
        (*m_root_data)["Type"].append("Space");
        (*m_root_data)["Flag"].append("ComponentManager");
        (*m_root_data)["Flag"].append("ObjectManager");
        (*m_root_data)["Flag"].append("Scene");
        (*m_root_data)["Flag"].append("World");
        (*m_root_data)["Settings"]["ProjectionMatrix"] = "Perspective";
    }
}
