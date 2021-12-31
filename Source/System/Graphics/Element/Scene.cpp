#include "Scene.hpp"

#include "SkinnedMesh.hpp"
#include "Light.hpp"
#include "../Common/Renderer/RendererCommon.hpp"
#include "../Common/Shader/ShaderManagerCommon.hpp"
#include "../Utility/MatrixManager.hpp"
#include "../Utility/PrimitiveRenderer.hpp"
#include "ParticleEmitter.hpp"
#include "../../../Manager/Resource/ResourceManager.hpp"
#include "Mesh.hpp"
#include "CubeMapSky.hpp"
#include "../Utility/TextRenderer.hpp"
#include "../Common/Buffer2/ConstantBufferCommon.hpp"
#include "../Common/Buffer2/ConstantBufferData.hpp"
#include "TextSprite.hpp"
#include "../../../Manager/Resource/ResourceType/MeshResource.hpp"
#include "../../../Manager/Resource/ResourceType/TextureResource.hpp"
#include "../../Core/Utility/CoreUtility.hpp"
#include "../DataType/MatrixData.hpp"
#include "../Utility/MaterialManager.hpp"

namespace CS460
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::Initialize()
    {
        m_matrix_manager->AddScene(this);
        //primitive renderer
        m_primitive_renderer = new PrimitiveRenderer(m_renderer);
        m_primitive_renderer->Initialize(m_shader_manager->GetShader("Color"));
        m_color_ins_shader = m_shader_manager->GetShader("ColorInstancing");
        UpdateView();
        UpdateProjection();

        m_matrix_buffer = new ConstantBufferCommon();
        m_matrix_buffer->Init(m_renderer, eBindingStage::VertexShader, sizeof(MatrixBufferData), 0);

        m_matrix_instancing_buffer = new ConstantBufferCommon();
        m_matrix_instancing_buffer->Init(m_renderer, eBindingStage::VertexShader, sizeof(MatrixBufferDataInstancing), 0);

        m_camera_buffer = new ConstantBufferCommon();
        m_camera_buffer->Init(m_renderer, eBindingStage::PixelShader, sizeof(CameraBufferData), 1);

        m_light_buffer = new ConstantBufferCommon();
        m_light_buffer->Init(m_renderer, eBindingStage::PixelShader, sizeof(MultipleLightsBufferData), 2);
    }

    void Scene::Update(Real dt)
    {
        if (m_main_camera == nullptr || m_cameras.empty())
            return;
        m_main_camera->Update();

        CameraBufferData camera_data;
        camera_data.camera_position = m_main_camera->GetPosition();
        m_camera_buffer->Update(camera_data);

        MatrixBufferData mvp_buffer;
        mvp_buffer.view = m_main_camera->GetViewMatrix();
        mvp_buffer.proj = m_projection_matrix;
        m_matrix_buffer->Update(mvp_buffer);

        MatrixBufferDataInstancing vp_data;
        vp_data.view = m_main_camera->GetViewMatrix();
        vp_data.proj = m_projection_matrix;
        m_matrix_instancing_buffer->Update(vp_data);

        m_frustum.ConstructFrustum(m_matrix_manager->GetFarPlane(), mvp_buffer.proj, mvp_buffer.view);
        m_primitive_renderer->UpdateFrustum(m_frustum);
        for (auto& particle : m_particle_emitters)
        {
            particle->Update(dt);
        }

        for (auto& mesh : m_meshes)
        {
            mesh->ClearCount();
        }

        for (auto& mesh_compo : m_mesh_components)
        {
            mesh_compo->Update(dt);
        }

        for (auto& light : m_lights)
        {
            light->Update(dt);
        }

        for (auto& sky_box : m_cube_map_skies)
        {
            sky_box->SetCamera(m_main_camera);
            sky_box->Update(dt);
        }

        for (auto& skinned_mesh : m_skinned_meshes)
        {
            skinned_mesh->UpdateMatrixBuffer(mvp_buffer.view, mvp_buffer.proj);
            skinned_mesh->Update(dt);
        }
    }

    void Scene::Render() const
    {
        if (m_main_camera == nullptr || m_cameras.empty())
            return;

        m_primitive_renderer->Render(m_matrix_buffer);
        m_primitive_renderer->RenderInstancing(m_color_ins_shader, m_matrix_instancing_buffer);
        m_primitive_renderer->Clear();

        {
            int count = (int)m_lights.size();
            count     = count < 16 ? count : 16;
            MultipleLightsBufferData light_buffer_data;
            light_buffer_data.light_count = count;
            for (int i = 0; i < count; ++i)
            {
                light_buffer_data.data[i] = m_lights[i]->GetLightBuffer();
            }
            m_light_buffer->Update(light_buffer_data);
        }

        for (auto& mesh : m_meshes)
        {
            mesh->Bind();
            m_matrix_instancing_buffer->Bind();

            std::string type = mesh->GetShaderType();

            if (type == "PhongInstancing")
            {
                m_camera_buffer->Bind();
                m_light_buffer->Bind();
            }

            m_shader_manager->Bind(type);
            mesh->Draw();
        }

        for (auto& skinned_mesh : m_skinned_meshes)
        {
            size_t count = skinned_mesh->SubMeshCount();

            for (size_t i = 0; i < count; ++i)
            {
                skinned_mesh->Bind(i);
                std::string type = skinned_mesh->GetShaderType();
                if (type == "SkinnedPhong")
                {
                    m_camera_buffer->Bind();
                    m_light_buffer->Bind();
                }
                m_shader_manager->Bind(type);
                skinned_mesh->Draw(i);
            }
        }

        for (auto& particle : m_particle_emitters)
        {
            particle->SetBillboardPosition(m_main_camera->GetPosition());
            particle->Bind();
            m_matrix_instancing_buffer->Bind();
            m_shader_manager->Bind("TextureInstancing");
            particle->Draw();
        }

        //m_shader_manager->Bind("ColorInstancing");
        //m_test_mesh->Render();
        //for (auto& text_sprite : m_text_sprites)
        //{
        //    mvp_data.model = text_sprite->GetModelMatrix();
        //    text_sprite->Render();
        //    m_shader_manager->RenderTextureShader(
        //                                          text_sprite->GetIndexCount(),
        //                                          mvp_data,
        //                                          text_sprite->GetTexture(),
        //                                          text_sprite->GetColor());
        //}

        //sky-box
        {
            m_renderer->SetCullMode(false);
            m_renderer->SetDSMode(false);
            for (auto& sky_box : m_cube_map_skies)
            {
                MatrixBufferData mvp_buffer;
                mvp_buffer.world = sky_box->GetModelMatrix();
                mvp_buffer.view  = m_main_camera->GetViewMatrix();
                mvp_buffer.proj  = m_projection_matrix;
                m_matrix_buffer->Update(mvp_buffer);

                sky_box->Bind();
                m_matrix_buffer->Bind();
                m_shader_manager->Bind("SkyBox");
                sky_box->Draw();
            }

            m_renderer->SetCullMode(true);
            m_renderer->SetDSMode(true);
        }
    }

    void Scene::Shutdown()
    {
        {
            m_matrix_buffer->Shutdown();
            delete m_matrix_buffer;
            m_matrix_buffer = nullptr;

            m_matrix_instancing_buffer->Shutdown();
            delete m_matrix_instancing_buffer;
            m_matrix_instancing_buffer = nullptr;

            m_camera_buffer->Shutdown();
            delete m_camera_buffer;
            m_camera_buffer = nullptr;

            m_light_buffer->Shutdown();
            delete m_light_buffer;
            m_light_buffer = nullptr;
        }

        m_matrix_manager->RemoveScene(this);
        if (m_primitive_renderer != nullptr)
        {
            m_primitive_renderer->Shutdown();
            delete m_primitive_renderer;
            m_primitive_renderer = nullptr;
        }

        //clear cameras
        {
            for (auto& camera : m_cameras)
            {
                camera->Shutdown();
                delete camera;
                camera = nullptr;
            }
            m_cameras.clear();
        }

        //clear text sprites
        {
            for (auto& text_sprite : m_text_sprites)
            {
                text_sprite->Shutdown();
                delete text_sprite;
                text_sprite = nullptr;
            }
            m_text_sprites.clear();
        }

        //clear particle emitters
        {
            for (auto& particle : m_particle_emitters)
            {
                particle->Shutdown();
                delete particle;
                particle = nullptr;
            }
            m_particle_emitters.clear();
        }

        //clear lights
        {
            for (auto& light : m_lights)
            {
                light->Shutdown();
                delete light;
                light = nullptr;
            }
            m_lights.clear();
        }

        //clear ani_meshes
        {
            for (auto& ani_mesh : m_skinned_meshes)
            {
                ani_mesh->Shutdown();
                delete ani_mesh;
                ani_mesh = nullptr;
            }
            m_skinned_meshes.clear();
        }

        //clear cube_map_skies
        {
            for (auto& sky : m_cube_map_skies)
            {
                sky->Shutdown();
                delete sky;
                sky = nullptr;
            }
            m_cube_map_skies.clear();
        }

        //clear meshes
        {
            for (auto& [fst1, mesh_table] : m_mesh_table)
            {
                mesh_table.clear();
            }
            m_mesh_table.clear();

            for (auto& mesh : m_meshes)
            {
                mesh->Shutdown();
                delete mesh;
                mesh = nullptr;
            }
            m_meshes.clear();
            m_mesh_components.clear();
        }
    }

    void Scene::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
    }

    void Scene::SetShaderManager(ShaderManagerCommon* shader_manager)
    {
        m_shader_manager = shader_manager;
    }

    void Scene::SetMatrixManager(MatrixManager* matrix_manager)
    {
        m_matrix_manager = matrix_manager;
    }

    void Scene::SetMainCamera(Camera* camera)
    {
        if (camera != nullptr)
        {
            m_main_camera = camera;
        }
    }

    void Scene::SetResourceManager(ResourceManager* resource_manager)
    {
        m_resource_manager = resource_manager;
    }

    void Scene::SetTextRenderer(TextRenderer* text_renderer)
    {
        m_text_renderer = text_renderer;
    }

    void Scene::SetMaterialManager(MaterialManager* material_manager)
    {
        m_material_manager = material_manager;
    }

    void Scene::SetProjectionType(eProjectionType projection_type)
    {
        m_projection_type = projection_type;
        UpdateProjection();
    }

    void Scene::UpdateView()
    {
        if (m_main_camera != nullptr)
        {
            m_view_matrix = m_main_camera->GetViewMatrix();
        }
        if (m_primitive_renderer != nullptr)
        {
            m_primitive_renderer->UpdateViewMatrix(m_view_matrix);
        }
    }

    void Scene::UpdateProjection()
    {
        switch (m_projection_type)
        {
        case eProjectionType::Perspective:
            m_projection_matrix = m_matrix_manager->GetPerspectiveMatrix();
            m_orthogonal_matrix = m_matrix_manager->GetOrthoGraphicMatrix();
            break;
        case eProjectionType::OrthoGraphic:
            m_projection_matrix = m_matrix_manager->GetOrthoGraphicMatrix();
            break;
        default:
            break;
        }
        if (m_primitive_renderer != nullptr)
        {
            m_primitive_renderer->UpdateProjectionMatrix(m_projection_matrix);
        }
    }

    void Scene::OnResize() const
    {
        /* if (m_deferred_buffer != nullptr)
         {
             m_deferred_buffer->OnResize(
                                         m_matrix_manager->GetScreenWidth(),
                                         m_matrix_manager->GetScreenHeight());
         }*/
    }

    PrimitiveRenderer* Scene::GetPrimitiveRenderer() const
    {
        return m_primitive_renderer;
    }

    Mesh* Scene::GetMesh(size_t model_id, size_t material_id)
    {
        auto found_model = m_mesh_table.find(model_id);
        if (found_model != m_mesh_table.end())
        {
            auto& material_table = found_model->second;
            auto  found_material = material_table.find(material_id);

            if (found_material != material_table.end())
            {
                return found_material->second;
            }
        }
        return nullptr;
    }

    Mesh* Scene::AddMesh(const std::string& model_path, const MaterialIdentifier& material)
    {
        Mesh*         created;
        MeshResource* model_resource = m_resource_manager->GetMeshResource(ToWString(model_path));
        size_t        model_id       = reinterpret_cast<size_t>(model_resource);
        size_t        material_id    = m_material_manager->GetID(material);

        auto found_model = m_mesh_table.find(model_id);

        if (found_model == m_mesh_table.end())
        {
            //add new
            created = new Mesh();
            created->SetRenderer(m_renderer);
            SetUpMesh(created, model_resource->GetMeshData(), material, model_id, material_id);
            m_mesh_table.emplace(model_id, std::unordered_map<size_t, Mesh*>({{material_id, created}}));
            m_meshes.push_back(created);
        }
        else
        {
            auto& material_table = found_model->second;
            auto  found_material = material_table.find(material_id);

            if (found_material == material_table.end())
            {
                //add new
                created = new Mesh();
                created->SetRenderer(m_renderer);
                SetUpMesh(created, model_resource->GetMeshData(), material, model_id, material_id);
                material_table.emplace(material_id, created);
                m_meshes.push_back(created);
            }
            else
            {
                //found duplicated
                created = found_material->second;
            }
        }

        return created;
    }

    Mesh* Scene::AddMesh(MeshResource* model_resource, const MaterialIdentifier& material)
    {
        Mesh*  created;
        size_t model_id    = reinterpret_cast<size_t>(model_resource);
        size_t material_id = m_material_manager->GetID(material);

        auto found_model = m_mesh_table.find(model_id);

        if (found_model == m_mesh_table.end())
        {
            //add new
            created = new Mesh();
            created->SetRenderer(m_renderer);
            SetUpMesh(created, model_resource->GetMeshData(), material, model_id, material_id);
            m_mesh_table.emplace(model_id, std::unordered_map<size_t, Mesh*>({{material_id, created}}));
            m_meshes.push_back(created);
        }
        else
        {
            auto& material_table = found_model->second;
            auto  found_material = material_table.find(material_id);

            if (found_material == material_table.end())
            {
                //add new
                created = new Mesh();
                created->SetRenderer(m_renderer);
                SetUpMesh(created, model_resource->GetMeshData(), material, model_id, material_id);
                material_table.emplace(material_id, created);
                m_meshes.push_back(created);
            }
            else
            {
                //found duplicated
                created = found_material->second;
            }
        }

        return created;
    }

    Mesh* Scene::AddMesh(MeshData* model_data, const MaterialIdentifier& material)
    {
        Mesh*  created;
        size_t model_id    = reinterpret_cast<size_t>(model_data);
        size_t material_id = m_material_manager->GetID(material);
        auto   found_model = m_mesh_table.find(model_id);

        if (found_model == m_mesh_table.end())
        {
            //add new
            created = new Mesh();
            created->SetRenderer(m_renderer);
            SetUpMesh(created, model_data, material, model_id, material_id);
            m_mesh_table.emplace(model_id, std::unordered_map<size_t, Mesh*>({{material_id, created}}));
            m_meshes.push_back(created);
        }
        else
        {
            auto& material_table = found_model->second;
            auto  found_material = material_table.find(material_id);

            if (found_material == material_table.end())
            {
                //add new
                created = new Mesh();
                created->SetRenderer(m_renderer);
                SetUpMesh(created, model_data, material, model_id, material_id);
                material_table.emplace(material_id, created);
                m_meshes.push_back(created);
            }
            else
            {
                //found duplicated
                created = found_material->second;
            }
        }

        return created;
    }

    void Scene::AddMesh(MeshComponent* mesh_compo)
    {
        m_mesh_components.push_back(mesh_compo);
    }

    void Scene::RemoveMesh(MeshComponent* mesh_compo)
    {
        if (!m_mesh_components.empty())
        {
            auto found = std::find(m_mesh_components.begin(), m_mesh_components.end(), mesh_compo);
            m_mesh_components.erase(found);
        }
    }

    void Scene::AddLight(Light* light)
    {
        m_lights.push_back(light);
    }

    void Scene::RemoveLight(Light* light)
    {
        if (!m_lights.empty())
        {
            auto found = std::find(m_lights.begin(), m_lights.end(), light);
            m_lights.erase(found);
        }
    }

    void Scene::AddCubeMapSky(CubeMapSky* sky)
    {
        sky->SetRenderer(m_renderer);
        m_cube_map_skies.push_back(sky);
    }

    void Scene::RemoveCubeMapSky(CubeMapSky* sky)
    {
        if (!m_cube_map_skies.empty())
        {
            auto found = std::find(m_cube_map_skies.begin(), m_cube_map_skies.end(), sky);
            m_cube_map_skies.erase(found);
        }
    }

    void Scene::AddSkyDome(TextureSky* sky)
    {
        //m_sky_domes.push_back(sky_dome);
    }

    void Scene::RemoveSkyDome(TextureSky* sky)
    {
        //if (!m_sky_domes.empty())
        //{
        //    auto found = std::find(m_sky_domes.begin(), m_sky_domes.end(), sky_dome);
        //    m_sky_domes.erase(found);
        //}
    }

    void Scene::AddAniMesh(SkinnedMesh* ani_mesh)
    {
        ani_mesh->SetRenderer(m_renderer);
        auto found = std::find(m_skinned_meshes.begin(), m_skinned_meshes.end(), ani_mesh);
        if (found == m_skinned_meshes.end())
        {
            m_skinned_meshes.push_back(ani_mesh);
        }
    }

    void Scene::RemoveAniMesh(SkinnedMesh* ani_mesh)
    {
        if (!m_skinned_meshes.empty())
        {
            auto found = std::find(m_skinned_meshes.begin(), m_skinned_meshes.end(), ani_mesh);
            m_skinned_meshes.erase(found);
        }
    }

    Camera* Scene::AddCamera(Camera* camera)
    {
        if (m_cameras.empty())
        {
            m_main_camera = camera;
        }
        m_cameras.push_back(camera);
        return camera;
    }

    void Scene::AddTextSprite(TextSprite* text_sprite)
    {
        m_text_sprites.push_back(text_sprite);
    }

    void Scene::AddParticleEmitter(ParticleEmitter* particle_emitter)
    {
        m_particle_emitters.push_back(particle_emitter);
    }

    void Scene::RemoveCamera(Camera* camera)
    {
        auto found = std::find(m_cameras.begin(), m_cameras.end(), camera);
        m_cameras.erase(found);
    }

    void Scene::RemoveTextSprite(TextSprite* text_sprite)
    {
        auto found = std::find(m_text_sprites.begin(), m_text_sprites.end(), text_sprite);
        m_text_sprites.erase(found);
    }

    void Scene::RemoveParticleEmitter(ParticleEmitter* particle_emitter)
    {
        auto found = std::find(m_particle_emitters.begin(), m_particle_emitters.end(), particle_emitter);
        m_particle_emitters.erase(found);
    }

    void Scene::InitializeTextSprite(TextSprite* text_sprite) const
    {
        text_sprite->Initialize(m_renderer);
    }

    void Scene::InitializeParticleEmitter(ParticleEmitter* particle_emitter) const
    {
        particle_emitter->SetRenderer(m_renderer);
        particle_emitter->SetBuffer();
    }

    Basis Scene::GetMainCameraBasis() const
    {
        return m_main_camera->GetBasis();
    }

    Real Scene::GetAspectRatio() const
    {
        return m_matrix_manager->GetAspectRatio();
    }

    Ray Scene::GetPickingRay(const Vector2& pos) const
    {
        Matrix33 proj(m_projection_matrix);
        Vector2  point;
        point.x = pos.x / proj(0, 0);
        point.y = pos.y / proj(1, 1);

        Matrix33 inv_view(m_main_camera->GetViewMatrix().Inverse());
        Vector3  origin(m_main_camera->GetPosition());
        Vector3  dir;
        dir.x = (point.x * inv_view(0, 0)) + (point.y * inv_view(1, 0)) + inv_view(2, 0);
        dir.y = (point.x * inv_view(0, 1)) + (point.y * inv_view(1, 1)) + inv_view(2, 1);
        dir.z = (point.x * inv_view(0, 2)) + (point.y * inv_view(1, 2)) + inv_view(2, 2);

        //m_main_camera->GetTransform()->LocalToWorldMatrix();

        dir.SetNormalize();

        Ray ray(origin, dir);
        return ray;
    }

    void Scene::SetUpMesh(Mesh* mesh, MeshData* model_data, const MaterialIdentifier& material, size_t model_id, size_t material_id) const
    {
        if (mesh != nullptr)
        {
            mesh->SetModelData(model_data);
            mesh->SetSceneID(model_id, material_id);
            mesh->SetMaterialIdentifier(material);
            //diffuse texture0
            if (material.diffuse0 != "")
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(ToWString(material.diffuse0))->GetTexture());
            }
            else
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(L"DefaultTexture")->GetTexture());
            }
            //diffuse texture1
            if (material.diffuse1 != "")
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(ToWString(material.diffuse1))->GetTexture());
            }
            else
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(L"DefaultTexture")->GetTexture());
            }
            //diffuse texture2
            if (material.diffuse2 != "")
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(ToWString(material.diffuse2))->GetTexture());
            }
            else
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(L"DefaultTexture")->GetTexture());
            }
            //specular texture
            if (material.specular0 != "")
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(ToWString(material.specular0))->GetTexture());
            }
            else
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(L"DefaultTexture")->GetTexture());
            }
            //normal texture
            if (material.normal0 != "")
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(ToWString(material.normal0))->GetTexture());
            }
            else
            {
                mesh->AddTexture(m_resource_manager->GetTextureResource(L"DefaultTexture")->GetTexture());
            }
        }
    }

    void Scene::UpdateMesh(Real dt)
    {
        for (auto& mesh_component : m_mesh_components)
        {
            //do mesh_compo
            mesh_component->Update(dt);
        }
    }
}
