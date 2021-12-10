#include "MeshComponent.hpp"
#include "../../Object/Object.hpp"
#include "TransformComponent.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Graphics/Element/Mesh.hpp"
#include "../../Resource/ResourceManager.hpp"
#include "../../Resource/ResourceType/MeshResource.hpp"

namespace CS460
{
    MeshComponent::~MeshComponent()
    {
    }

    void MeshComponent::Initialize()
    {
        Subscribe();
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
            }
        }
    }

    void MeshComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);

        if (m_mesh != nullptr)
        {
            m_mesh->AddInstance(m_transform, m_material_color);
        }
    }

    void MeshComponent::Shutdown()
    {
        Unsubscribe();
    }

    void MeshComponent::SetDiffuseColor(const Color& color)
    {
        m_material_color.diffuse = color;
    }

    bool MeshComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Texture"))
        {
            auto texture_data = data["Texture"];

            if (JsonResource::HasMember(texture_data, "Diffuse") && texture_data["Diffuse"].isArray())
            {
                int idx = 0;
                for (auto it = texture_data["Diffuse"].begin(); it != texture_data["Diffuse"].end(); ++it)
                {
                    if ((*it).isString())
                    {
                        if (idx == 0)
                        {
                            m_material_identifier.diffuse0 = (*it).asString();
                        }
                        if (idx == 1)
                        {
                            m_material_identifier.diffuse1 = (*it).asString();
                        }
                        if (idx == 2)
                        {
                            m_material_identifier.diffuse2 = (*it).asString();
                        }
                    }
                    idx++;
                }
            }

            if (JsonResource::HasMember(texture_data, "Diffuse0"))
            {
                if (texture_data["Diffuse0"].isString())
                {
                    m_material_identifier.diffuse0 = texture_data["Diffuse0"].asString();
                }
            }

            if (JsonResource::HasMember(texture_data, "Diffuse1"))
            {
                if (texture_data["Diffuse1"].isString())
                {
                    m_material_identifier.diffuse1 = texture_data["Diffuse1"].asString();
                }
            }

            if (JsonResource::HasMember(texture_data, "Diffuse2"))
            {
                if (texture_data["Diffuse2"].isString())
                {
                    m_material_identifier.diffuse2 = texture_data["Diffuse2"].asString();
                }
            }

            if (JsonResource::HasMember(texture_data, "DiffuseType"))
            {
                if (texture_data["DiffuseType"].isString())
                {
                    std::string type = texture_data["DiffuseType"].asString();
                    if (type == "Texture")
                    {
                        m_material_identifier.diffuse_type = 1;
                    }
                    else if (type == "AlphaMapping")
                    {
                        m_material_identifier.diffuse_type = 2;
                    }
                    else if (type == "LightMapping")
                    {
                        m_material_identifier.diffuse_type = 3;
                    }
                    else if (type == "Multi-Texture")
                    {
                        m_material_identifier.diffuse_type = 4;
                    }
                    else
                    {
                        m_material_identifier.diffuse_type = 0;
                    }
                }
            }

            if (JsonResource::HasMember(texture_data, "Specular"))
            {
                if (texture_data["Specular"].isString())
                {
                    m_material_identifier.specular0     = texture_data["Specular"].asString();
                    m_material_identifier.specular_type = 1;
                }
                else
                {
                    m_material_identifier.specular_type = 0;
                }
            }
            else
            {
                m_material_identifier.specular_type = 0;
            }

            if (JsonResource::HasMember(texture_data, "NormalMap"))
            {
                if (texture_data["NormalMap"].isString())
                {
                    m_material_identifier.normal0     = texture_data["NormalMap"].asString();
                    m_material_identifier.normal_type = 1;
                }
                else
                {
                    m_material_identifier.normal_type = 0;
                }
            }
            else
            {
                m_material_identifier.normal_type = 0;
            }
        }

        if (JsonResource::HasMember(data, "Color"))
        {
            auto color_data = data["Color"];

            if (JsonResource::IsColor(color_data["Ambient"]))
            {
                m_material_color.ambient = JsonResource::AsColor(color_data["Ambient"]);
            }

            if (JsonResource::IsColor(color_data["Diffuse"]))
            {
                m_material_color.diffuse = JsonResource::AsColor(color_data["Diffuse"]);
            }

            if (JsonResource::IsColor(color_data["Specular"]))
            {
                m_material_color.specular = JsonResource::AsColor(color_data["Specular"]);
            }
        }

        if (JsonResource::HasMember(data, "Shader Type"))
        {
            m_material_identifier.shader_type = data["Shader Type"].asString();
        }
        else
        {
            //E5_TODO : need to update default shader name.
            m_material_identifier.shader_type = "Default";
        }

        if (JsonResource::HasMember(data, "Mesh"))
        {
            auto        mesh_data = data["Mesh"];
            std::string mesh_type;
            if (JsonResource::HasMember(mesh_data, "Type"))
            {
                mesh_type = mesh_data["Type"].asString();
            }

            if (mesh_data["Path"].isString() && mesh_type == "File")
            {
                m_model_resource_path = mesh_data["Path"].asString();
                m_model_resource      = m_space->GetResourceManager()->GetMeshResource(ToWString(m_model_resource_path));
                m_model_id            = reinterpret_cast<size_t>(m_model_resource);
            }
            else if (mesh_type == "Generate")
            {
                //E5_TODO : Need to add generated mesh
                //
            }
            else
            {
                //default mesh
            }
            AddMesh();
            //E5_TODO : Need to init properly Mesh2. 
        }
        return true;
    }

    void MeshComponent::Save(Json::Value& data) const
    {
    }

    void MeshComponent::Edit(CommandRegistry* command_registry)
    {
        if (ImGui::CollapsingHeader(m_type.c_str(), &m_b_open))
        {
            Matrix44 model = m_transform->LocalToWorldMatrix().Transpose();

            static bool is_1f = true;

            if (ImGui::Button(is_1f ? "3digit" : "1digit"))
            {
                is_1f = !is_1f;
            }

            if (is_1f)
            {
                ImGui::Text("World");
                ImGui::Text("|%.1f, %.1f, %.1f, %.1f|", model[0], model[1], model[2], model[3]);
                ImGui::Text("|%.1f, %.1f, %.1f, %.1f|", model[4], model[5], model[6], model[7]);
                ImGui::Text("|%.1f, %.1f, %.1f, %.1f|", model[8], model[9], model[10], model[11]);
                ImGui::Text("|%.1f, %.1f, %.1f, %.1f|", model[12], model[13], model[14], model[15]);

                model.SetInverse();
                model.SetTranspose();
                ImGui::Text("World Inverse Transpose");
                ImGui::Text("|%.1f, %.1f, %.1f, %.1f|", model[0], model[1], model[2], model[3]);
                ImGui::Text("|%.1f, %.1f, %.1f, %.1f|", model[4], model[5], model[6], model[7]);
                ImGui::Text("|%.1f, %.1f, %.1f, %.1f|", model[8], model[9], model[10], model[11]);
                ImGui::Text("|%.1f, %.1f, %.1f, %.1f|", model[12], model[13], model[14], model[15]);
            }
            else
            {
                ImGui::Text("World");
                ImGui::Text("|%.3f, %.3f, %.3f, %.3f|", model[0], model[1], model[2], model[3]);
                ImGui::Text("|%.3f, %.3f, %.3f, %.3f|", model[4], model[5], model[6], model[7]);
                ImGui::Text("|%.3f, %.3f, %.3f, %.3f|", model[8], model[9], model[10], model[11]);
                ImGui::Text("|%.3f, %.3f, %.3f, %.3f|", model[12], model[13], model[14], model[15]);

                model.SetInverse();
                model.SetTranspose();
                ImGui::Text("World Inverse Transpose");
                ImGui::Text("|%.3f, %.3f, %.3f, %.3f|", model[0], model[1], model[2], model[3]);
                ImGui::Text("|%.3f, %.3f, %.3f, %.3f|", model[4], model[5], model[6], model[7]);
                ImGui::Text("|%.3f, %.3f, %.3f, %.3f|", model[8], model[9], model[10], model[11]);
                ImGui::Text("|%.3f, %.3f, %.3f, %.3f|", model[12], model[13], model[14], model[15]);
            }
        }
        //E5_TODO : Need to add user interface for mesh component
        //E5_TODO : Select Model Data from file
        //E5_TODO : Select Generated Model Data
        //E5_TODO : Select Texture Material
        //E5_TODO : Select Color Material
    }

    void MeshComponent::Subscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetScene()->AddMesh(this);
        }
    }

    void MeshComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetScene()->RemoveMesh(this);
        }
    }

    void MeshComponent::AddMesh()
    {
        auto scene = m_space->GetScene();
        if (m_model_resource != nullptr)
        {
            m_mesh = scene->AddMesh(m_model_resource, m_material_identifier);
        }
        else if (m_model_resource_path != "")
        {
            m_mesh = scene->AddMesh(m_model_resource_path, m_material_identifier);
        }
        else
        {
            //E5_TODO : Need to add Generated Mesh Creation
        }
    }

    MeshComponent::MeshComponent(Object* owner)
        : Component(owner)
    {
    }

    void MeshComponent::Clone(MeshComponent* origin)
    {
        if (origin != nullptr && origin != this)
        {
            //copy data
        }
    }
}
