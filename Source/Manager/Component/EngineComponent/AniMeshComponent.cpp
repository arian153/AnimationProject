#include "AniMeshComponent.hpp"

#include "TransformComponent.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Animation/AnimationSubsystem.hpp"
#include "../../../System/Animation/Skeleton/Skeleton.hpp"
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

            ImGui::Text("Bone Mode");

            const char* bone_mode[] = { "VQS", "MAT", "BIND" };

            if (ImGui::Combo("##Bone Type", &m_skeleton->m_render_mode, bone_mode, 3))
            {
            }


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
