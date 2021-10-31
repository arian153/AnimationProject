#include "AniMeshComponent.hpp"

#include "TransformComponent.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Animation/AnimationSubsystem.hpp"
#include "../../../System/Animation/Skeleton/Skeleton.hpp"
#include "../../../System/Animation/Space/AnimationSpace.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../../System/Graphics/Element/SkinnedMesh.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../Object/Object.hpp"
#include "../../Resource/ResourceManager.hpp"
#include "../../Resource/ResourceType/AniMeshResource.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../Resource/ResourceType/TextureResource.hpp"
#include "../../Space/Space.hpp"

namespace CS460
{
    AniMeshComponent::~AniMeshComponent()
    {
    }

    void AniMeshComponent::Initialize()
    {
        if (m_ani_mesh == nullptr)
        {
            m_ani_mesh              = new SkinnedMesh();
            m_ani_mesh->m_component = this;
        }

        if (m_skeleton == nullptr)
        {
            m_skeleton              = new Skeleton();
            m_skeleton->m_component = this;
        }

        Subscribe();

        if (m_ani_mesh != nullptr)
        {
            m_ani_mesh->Initialize();
            m_ani_mesh->CreateBuffer();
        }

        if (m_skeleton != nullptr)
        {
            m_skeleton->Initialize();
        }

        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform             = m_owner->GetComponent<TransformComponent>()->GetTransform();
                m_skeleton->m_transform = m_transform;
                m_ani_mesh->m_transform = m_transform;
            }
        }
    }

    void AniMeshComponent::Update(Real dt)
    {
        if (m_ani_mesh != nullptr && m_skeleton != nullptr)
        {
            m_ani_mesh->UpdateSkeletonTransforms(m_skeleton->m_final_vqs);
        }
    }

    void AniMeshComponent::Shutdown()
    {
        Unsubscribe();
        m_sub_materials.clear();
        if (m_ani_mesh != nullptr)
        {
            m_ani_mesh->Shutdown();
            delete m_ani_mesh;
            m_ani_mesh = nullptr;
        }

        if (m_skeleton != nullptr)
        {
            m_skeleton->Shutdown();
            delete m_skeleton;
            m_skeleton = nullptr;
        }
    }

    bool AniMeshComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Shader Type"))
        {
            m_ani_mesh->m_shader_type = data["Shader Type"].asString();
        }

        if (JsonResource::HasMember(data, "Mesh"))
        {
            auto mesh_data = data["Mesh"];

            if (mesh_data["Path"].isString())
            {
                m_mesh_resource_path = mesh_data["Path"].asString();
                m_mesh_resource      = m_space->GetResourceManager()->GetAniMeshResource(ToWString(m_mesh_resource_path));

                if (m_mesh_resource != nullptr)
                {
                    m_mesh_resource->CopyData(m_ani_mesh);
                    m_mesh_resource->CopyData(m_skeleton);
                    m_skeleton->m_default_orientation = m_skeleton->m_transform->orientation;
                }
            }
        }

        if (JsonResource::HasMember(data, "Space Path"))
        {
            if (data["Space Path"].isArray())
            {
                Json::ArrayIndex size = data["Space Path"].size();

                for (Json::ArrayIndex i = 0; i < size; ++i)
                {
                    auto path_data = data["Space Path"][i];

                    if (JsonResource::HasMember(path_data, "Name"))
                    {
                        std::string path_name = path_data["Name"].asString();
                        int         path_idx  = m_skeleton->m_ani_space->GetPathIDX(path_name);
                        if (path_idx == -1)
                        {
                            //not found add
                            path_idx = m_skeleton->m_ani_space->CreateNewPath(path_name);
                            m_skeleton->AddPath(path_idx);
                            m_skeleton->m_ani_space->SetSkeleton(m_skeleton);

                            if (JsonResource::HasMember(path_data, "Data"))
                            {
                                if (path_data["Data"].isArray())
                                {
                                    Json::ArrayIndex control_points_count = path_data["Data"].size();

                                    for (Json::ArrayIndex j = 0; j < control_points_count; ++j)
                                    {
                                        if (JsonResource::IsVector3(path_data["Data"][j]))
                                        {
                                            m_skeleton->m_ani_space->AddControlPoint(JsonResource::AsVector3(path_data["Data"][j]));
                                        }
                                    }
                                }
                            }
                            m_skeleton->m_ani_space->SetEditable(false);
                        }
                        else
                        {
                            m_skeleton->AddPath(path_idx);
                        }
                    }

                    if (JsonResource::HasMember(path_data, "Duration") && path_data["Duration"].isNumeric())
                    {
                        m_skeleton->m_path_duration = path_data["Duration"].asFloat();
                    }

                    if (JsonResource::HasMember(path_data, "T1") && path_data["T1"].isNumeric() 
                        && JsonResource::HasMember(path_data, "T2") && path_data["T2"].isNumeric())
                    {
                        Real t1 = path_data["T1"].asFloat();
                        Real t2 = path_data["T2"].asFloat();

                        m_skeleton->m_speed_control.SetT1T2(t1, t2);
                    }
                }
            }
        }

        if (JsonResource::HasMember(data, "Material"))
        {
            if (data["Material"].isArray())
            {
                Json::ArrayIndex size = data["Material"].size();
                m_sub_materials.resize((size_t)size);
                if (size <= m_ani_mesh->SubMeshCount())
                {
                    for (Json::ArrayIndex i = 0; i < size; ++i)
                    {
                        auto texture_data = data["Material"][i]["Texture"];
                        if (JsonResource::HasMember(texture_data, "Diffuse") && texture_data["Diffuse"].isArray())
                        {
                            int idx = 0;
                            for (auto it = texture_data["Diffuse"].begin(); it != texture_data["Diffuse"].end(); ++it)
                            {
                                if ((*it).isString())
                                {
                                    if (idx == 0)
                                    {
                                        m_sub_materials[i].material_info.diffuse0 = (*it).asString();
                                    }
                                    if (idx == 1)
                                    {
                                        m_sub_materials[i].material_info.diffuse1 = (*it).asString();
                                    }
                                    if (idx == 2)
                                    {
                                        m_sub_materials[i].material_info.diffuse2 = (*it).asString();
                                    }
                                }
                                idx++;
                            }
                        }

                        if (JsonResource::HasMember(texture_data, "Diffuse0"))
                        {
                            if (texture_data["Diffuse0"].isString())
                            {
                                m_sub_materials[i].material_info.diffuse0 = texture_data["Diffuse0"].asString();
                            }
                        }

                        if (JsonResource::HasMember(texture_data, "Diffuse1"))
                        {
                            if (texture_data["Diffuse1"].isString())
                            {
                                m_sub_materials[i].material_info.diffuse0 = texture_data["Diffuse1"].asString();
                            }
                        }

                        if (JsonResource::HasMember(texture_data, "Diffuse2"))
                        {
                            if (texture_data["Diffuse2"].isString())
                            {
                                m_sub_materials[i].material_info.diffuse0 = texture_data["Diffuse2"].asString();
                            }
                        }

                        if (JsonResource::HasMember(texture_data, "DiffuseType"))
                        {
                            if (texture_data["DiffuseType"].isString())
                            {
                                std::string type = texture_data["DiffuseType"].asString();
                                if (type == "Texture")
                                {
                                    m_sub_materials[i].material_info.diffuse_type = 1;
                                }
                                else if (type == "AlphaMapping")
                                {
                                    m_sub_materials[i].material_info.diffuse_type = 2;
                                }
                                else if (type == "LightMapping")
                                {
                                    m_sub_materials[i].material_info.diffuse_type = 3;
                                }
                                else if (type == "Multi-Texture")
                                {
                                    m_sub_materials[i].material_info.diffuse_type = 4;
                                }
                                else
                                {
                                    m_sub_materials[i].material_info.diffuse_type = 0;
                                }
                            }
                        }

                        if (JsonResource::HasMember(texture_data, "Specular"))
                        {
                            if (texture_data["Specular"].isString())
                            {
                                m_sub_materials[i].material_info.specular0     = texture_data["Specular"].asString();
                                m_sub_materials[i].material_info.specular_type = 1;
                            }
                            else
                            {
                                m_sub_materials[i].material_info.specular_type = 0;
                            }
                        }
                        else
                        {
                            m_sub_materials[i].material_info.specular_type = 0;
                        }

                        if (JsonResource::HasMember(texture_data, "NormalMap"))
                        {
                            if (texture_data["NormalMap"].isString())
                            {
                                m_sub_materials[i].material_info.normal0     = texture_data["NormalMap"].asString();
                                m_sub_materials[i].material_info.normal_type = 1;
                            }
                            else
                            {
                                m_sub_materials[i].material_info.normal_type = 0;
                            }
                        }
                        else
                        {
                            m_sub_materials[i].material_info.normal_type = 0;
                        }
                        m_ani_mesh->SetMaterialIdentifier((int)i, m_sub_materials[i].material_info);

                        if (JsonResource::HasMember(data["Material"][i], "Color"))
                        {
                            auto color_data = data["Material"][i]["Color"];

                            if (JsonResource::IsColor(color_data["Ambient"]))
                            {
                                m_sub_materials[i].material_color.ambient = JsonResource::AsColor(color_data["Ambient"]);
                            }

                            if (JsonResource::IsColor(color_data["Diffuse"]))
                            {
                                m_sub_materials[i].material_color.diffuse = JsonResource::AsColor(color_data["Diffuse"]);
                            }

                            if (JsonResource::IsColor(color_data["Specular"]))
                            {
                                m_sub_materials[i].material_color.specular = JsonResource::AsColor(color_data["Specular"]);
                            }

                            if (JsonResource::IsColor(color_data["Reflect"]))
                            {
                                m_sub_materials[i].material_color.reflect = JsonResource::AsColor(color_data["Reflect"]);
                            }
                        }
                        m_ani_mesh->SetMaterialColor((int)i, m_sub_materials[i].material_color);

                        //diffuse texture0
                        if (m_sub_materials[i].material_info.diffuse0 != "")
                        {
                            m_ani_mesh->SetSubMeshTexture(
                                                          m_space->GetResourceManager()->GetTextureResource(
                                                                                                            ToWString(m_sub_materials[i].material_info.diffuse0))->GetTexture(), i, 0);
                        }
                        else
                        {
                            m_ani_mesh->SetSubMeshTexture(
                                                          m_space->GetResourceManager()->GetTextureResource(
                                                                                                            L"DefaultTexture")->GetTexture(), i, 0);
                        }
                        //diffuse texture1
                        if (m_sub_materials[i].material_info.diffuse1 != "")
                        {
                            m_ani_mesh->SetSubMeshTexture(
                                                          m_space->GetResourceManager()->GetTextureResource(
                                                                                                            ToWString(m_sub_materials[i].material_info.diffuse1))->GetTexture(), i, 1);
                        }
                        else
                        {
                            m_ani_mesh->SetSubMeshTexture(
                                                          m_space->GetResourceManager()->GetTextureResource(
                                                                                                            L"DefaultTexture")->GetTexture(), i, 1);
                        }
                        //diffuse texture2
                        if (m_sub_materials[i].material_info.diffuse2 != "")
                        {
                            m_ani_mesh->SetSubMeshTexture(
                                                          m_space->GetResourceManager()->GetTextureResource(
                                                                                                            ToWString(m_sub_materials[i].material_info.diffuse2))->GetTexture(), i, 2);
                        }
                        else
                        {
                            m_ani_mesh->SetSubMeshTexture(
                                                          m_space->GetResourceManager()->GetTextureResource(
                                                                                                            L"DefaultTexture")->GetTexture(), i, 2);
                        }
                        //specular texture
                        if (m_sub_materials[i].material_info.specular0 != "")
                        {
                            m_ani_mesh->SetSubMeshTexture(
                                                          m_space->GetResourceManager()->GetTextureResource(
                                                                                                            ToWString(m_sub_materials[i].material_info.specular0))->GetTexture(), i, 3);
                        }
                        else
                        {
                            m_ani_mesh->SetSubMeshTexture(
                                                          m_space->GetResourceManager()->GetTextureResource(
                                                                                                            L"DefaultTexture")->GetTexture(), i, 3);
                        }
                        //normal texture
                        if (m_sub_materials[i].material_info.normal0 != "")
                        {
                            m_ani_mesh->SetSubMeshTexture(
                                                          m_space->GetResourceManager()->GetTextureResource(
                                                                                                            ToWString(m_sub_materials[i].material_info.normal0))->GetTexture(), i, 4);
                        }
                        else
                        {
                            m_ani_mesh->SetSubMeshTexture(
                                                          m_space->GetResourceManager()->GetTextureResource(
                                                                                                            L"DefaultTexture")->GetTexture(), i, 4);
                        }
                    }
                }
            }
        }

        return true;
    }

    void AniMeshComponent::Save(Json::Value& data) const
    {
    }

    void AniMeshComponent::Edit(CommandRegistry* command_registry)
    {
        //GUI Editing.
        if (ImGui::CollapsingHeader(m_type.c_str(), &m_b_open))
        {
            ImGui::Text("Bone ");
            ImGui::SameLine();
            ImGui::Checkbox("##Draw Bone", &m_skeleton->m_b_draw);
            ImGui::Text("Bone Color");
            ImGui::ColorEdit4("##Bone Color", &m_skeleton->m_color.r);

            ImGui::Text("Animation : ");
            const char* pause_label = m_skeleton->m_b_pause ? "Play" : "Pause";
            ImGui::SameLine();
            if (ImGui::Button(pause_label))
            {
                m_skeleton->m_b_pause = !m_skeleton->m_b_pause;
            }

            int clip_count = (int)m_skeleton->m_clip_names.size();
            int clip_id    = m_skeleton->m_clip_id;
            if (ImGui::Combo("##Animation Clip", &m_skeleton->m_clip_id, VectorStringGetter, (void*)&m_skeleton->m_clip_names, clip_count))
            {
                clip_id = m_skeleton->m_clip_id;
            }

            ImGui::Text("Animation Speed ");
            ImGui::SameLine();
            if (ImGui::Button("Reset"))
            {
                m_skeleton->m_animation_clips[clip_id]->speed = 1.0f;
            }

            Real duration = m_skeleton->m_animation_clips[clip_id]->duration;
            ImGui::SliderFloat("##Animation Speed", &m_skeleton->m_animation_clips[clip_id]->speed, 0.1f * duration, 5.0f * duration);

            ImGui::Text("Track Status");
            if (ImGui::SliderFloat("##Elapsed time", &m_skeleton->m_animation_clips[clip_id]->elapsed_time, 0.0f, duration))
            {
                m_skeleton->m_animation_clips[clip_id]->UpdateTracks();
            }

            ImGui::Text("Select Current Path");
            int path_count = (int)m_skeleton->m_path_ids.size();
            int path_id    = m_skeleton->m_current_path;
            if (ImGui::Combo("##Animation Paths", &m_skeleton->m_current_path, VectorStringGetter, (void*)&m_skeleton->m_path_names, path_count))
            {
                path_id = m_skeleton->m_current_path;
                m_skeleton->m_ani_space->SetIDX(path_id);
                m_skeleton->m_ani_space->SetSkeleton(m_skeleton);
                m_skeleton->ResetStatus();
            }

            if (ImGui::Button("Create New Path"))
            {
                int id = m_skeleton->m_ani_space->CreateNewPath("");
                m_skeleton->AddPath(id);
                m_skeleton->m_ani_space->SetSkeleton(m_skeleton);
            }

            ImGui::Text("Add Other Path");
            int other_path_count = (int)m_skeleton->m_ani_space->path_names.size();
            int new_path_id      = 0;
            if (ImGui::Combo("##Animation Other Paths", &new_path_id, VectorStringGetter, (void*)&m_skeleton->m_ani_space->path_names, other_path_count))
            {
                m_skeleton->AddPath(new_path_id);
                m_skeleton->m_ani_space->SetIDX(new_path_id);
                m_skeleton->m_ani_space->SetSkeleton(m_skeleton);
                m_skeleton->ResetStatus();
            }

            ImGui::Text("Path Duration %f/%f", m_skeleton->m_speed_control.elapsed_t, m_skeleton->m_path_duration);
            if (ImGui::SliderFloat("Path Duration", &m_skeleton->m_path_duration, 0.1f, 60.0f))
            {
                m_skeleton->ResetStatus();
            }

            Real v0 = m_skeleton->m_speed_control.V0();
            Real t1 = m_skeleton->m_speed_control.T1();
            Real t2 = m_skeleton->m_speed_control.T2();

            ImGui::Text("T1 : %f", t1);
            if (ImGui::SliderFloat("##Speed Control T1", &t1, 0.001f, t2))
            {
                m_skeleton->m_speed_control.SetT1T2(t1, t2);
                m_skeleton->ResetStatus();
            }
            ImGui::Text("T2 : %f", t2);
            if (ImGui::SliderFloat("##Speed Control T2", &t2, t1, 0.999f))
            {
                m_skeleton->m_speed_control.SetT1T2(t1, t2);
                m_skeleton->ResetStatus();
            }

            ImGui::Text("V0 : %f", v0);
        }
    }

    void AniMeshComponent::Subscribe()
    {
        if (m_space != nullptr)
        {
            if (m_ani_mesh != nullptr)
            {
                m_space->GetScene()->AddAniMesh(m_ani_mesh);
            }

            if (m_skeleton != nullptr)
            {
                m_space->GetAnimationSubsystem()->AddSkeleton(m_skeleton);
            }
        }
    }

    void AniMeshComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            if (m_ani_mesh != nullptr)
            {
                m_space->GetScene()->RemoveAniMesh(m_ani_mesh);
            }

            if (m_skeleton != nullptr)
            {
                m_space->GetAnimationSubsystem()->RemoveSkeleton(m_skeleton);
            }
        }
    }

    AniMeshComponent::AniMeshComponent(Object* owner)
        : Component(owner)
    {
    }

    void AniMeshComponent::Clone(AniMeshComponent* origin)
    {
    }
}
