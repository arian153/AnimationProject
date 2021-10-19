#pragma once
#include <unordered_map>

#include "../../Math/Utility/MathDef.hpp"
#include "Camera.hpp"
#include "../DataType/ProjectionType.hpp"
#include <vector>
#include "../../../Manager/Component/EngineComponent/MeshComponent.hpp"
#include "../DataType/Frustum.hpp"

namespace CS460
{
    class SkinnedMesh;
    class TextureSky;
    class CubeMapSky;
    class Light;
    class MeshData;
    struct MaterialIdentifier;
    class MaterialManager;
    class VertexLayoutCommon;
    class ConstantBufferCommon;
    class ShaderProgramCommon;
    class Mesh;
    class TextRenderer;
    class ResourceManager;
    class ParticleEmitter;
    class TextSprite;
    class TextTextureBufferCommon;
    class RenderTextureBufferCommon;
    class DeferredBufferCommon;
    class ColliderSet;
    class PrimitiveRenderer;
    class MatrixManager;
    class ShaderManagerCommon;
    class RendererCommon;
    class RendererDX11;
    class DirectionalLight;
    class PointLight;
    class SpotLight;
    class CapsuleLight;

    using MeshTable = std::unordered_map<size_t, std::unordered_map<size_t, Mesh*>>;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void Initialize();
        void Update(Real dt);
        void Render() const;
        void Shutdown();

        void SetRenderer(RendererCommon* renderer);
        void SetShaderManager(ShaderManagerCommon* shader_manager);
        void SetMatrixManager(MatrixManager* matrix_manager);
        void SetMainCamera(Camera* camera);
        void SetResourceManager(ResourceManager* resource_manager);
        void SetTextRenderer(TextRenderer* text_renderer);
        void SetMaterialManager(MaterialManager* material_manager);

        void SetProjectionType(eProjectionType projection_type);
        void UpdateView();
        void UpdateProjection();
        void OnResize() const;

        PrimitiveRenderer* GetPrimitiveRenderer() const;

        Mesh* GetMesh(size_t model_id, size_t material_id);
        Mesh* AddMesh(const std::string& model_path, const MaterialIdentifier& material);
        Mesh* AddMesh(MeshResource* model_resource, const MaterialIdentifier& material);
        Mesh* AddMesh(MeshData* model_data, const MaterialIdentifier& material);
        void  AddMesh(MeshComponent* mesh_compo);
        void  RemoveMesh(MeshComponent* mesh_compo);

        void AddLight(Light* light);
        void RemoveLight(Light* light);
        void AddCubeMapSky(CubeMapSky* sky);
        void RemoveCubeMapSky(CubeMapSky* sky);
        void AddSkyDome(TextureSky* sky);
        void RemoveSkyDome(TextureSky* sky);

        void AddAniMesh(SkinnedMesh* ani_mesh);
        void RemoveAniMesh(SkinnedMesh* ani_mesh);

        //add
        Camera* AddCamera(Camera* camera);
        void    AddTextSprite(TextSprite* text_sprite);
        void    AddParticleEmitter(ParticleEmitter* particle_emitter);

        //remove
        void RemoveCamera(Camera* camera);
        void RemoveTextSprite(TextSprite* text_sprite);
        void RemoveParticleEmitter(ParticleEmitter* particle_emitter);

        void InitializeTextSprite(TextSprite* text_sprite) const;
        void InitializeParticleEmitter(ParticleEmitter* particle_emitter) const;

        Basis GetMainCameraBasis() const;
        Real  GetAspectRatio() const;
        Ray   GetPickingRay(const Vector2& pos) const;

    private:
        void SetUpMesh(Mesh* mesh, MeshData* model_data, const MaterialIdentifier& material, size_t model_id, size_t material_id) const;
        void UpdateMesh(Real dt);

    private:
        RendererCommon*      m_renderer           = nullptr;
        ShaderManagerCommon* m_shader_manager     = nullptr;
        MatrixManager*       m_matrix_manager     = nullptr;
        ResourceManager*     m_resource_manager   = nullptr;
        Camera*              m_main_camera        = nullptr;
        PrimitiveRenderer*   m_primitive_renderer = nullptr;
        TextRenderer*        m_text_renderer      = nullptr;
        MaterialManager*     m_material_manager   = nullptr;

        std::vector<Camera*>          m_cameras;
        std::vector<TextSprite*>      m_text_sprites;
        std::vector<ParticleEmitter*> m_particle_emitters;
        std::vector<MeshComponent*>   m_mesh_components;
        std::vector<Mesh*>            m_meshes;
        MeshTable                     m_mesh_table;

        std::vector<Light*>      m_lights;
        std::vector<CubeMapSky*> m_cube_map_skies;
        std::vector<SkinnedMesh*>    m_ani_meshes;

        eProjectionType m_projection_type = eProjectionType::Perspective;
        Matrix44        m_view_matrix;
        Matrix44        m_projection_matrix;
        Matrix44        m_orthogonal_matrix;
        Frustum         m_frustum;

        bool m_b_deferred_shading = false;

        ConstantBufferCommon* m_matrix_buffer            = nullptr;
        ConstantBufferCommon* m_matrix_instancing_buffer = nullptr;
        ConstantBufferCommon* m_camera_buffer            = nullptr;
        ConstantBufferCommon* m_light_buffer             = nullptr;
    };
}
