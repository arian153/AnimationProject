#pragma once
#include "Element/Scene.hpp"
#include <vector>
#include "../Math/Utility/MathDef.hpp"
#include "Utility/MatrixManager.hpp"
#include "DataType/Color.hpp"

namespace CS460
{
    class MaterialManager;
    class TextRenderer;
    class RenderTextureGenerator;
    class PrimitiveRenderer;
    class ResourceManager;
    class OSCommon;
    class RendererCommon;

    class RenderSystem
    {
    public:
        explicit RenderSystem(OSCommon* operating_system);
        ~RenderSystem();

        void Initialize(int rendering_width, int rendering_height, bool v_sync, ResourceManager* resource_manager);
        void Shutdown();

        void BeginUpdate() const;
        void EndUpdate() const;

        void OnResize(int width, int height);
        void OnFullscreen() const;

        void  SetVSyncEnable(bool vsync_flag);
        bool  IsVSyncEnable() const;
        void  SetFarNearPlane(Real far_plane, Real near_plane) const;
        void  SetFieldOfView(Real field_of_view) const;
        void  SetBackgroundColor(const Color& color);
        Color GetBackgroundColor() const;

        Scene* CreateScene();
        void   RemoveScene(Scene* scene);

        TextRenderer*           GetTextRenderer() const;
        RendererCommon*         GetRenderer() const;
        RenderTextureGenerator* GetRenderTextureGenerator() const;

    private:
        OSCommon*               m_operating_system         = nullptr;
        RendererCommon*         m_renderer                 = nullptr;
        ShaderManagerCommon*    m_shader_manager           = nullptr;
        MatrixManager*          m_matrix_manager           = nullptr;
        ResourceManager*        m_resource_manager         = nullptr;
        RenderTextureGenerator* m_render_texture_generator = nullptr;
        TextRenderer*           m_text_renderer            = nullptr;
        MaterialManager*        m_material_manger          = nullptr;

        std::vector<Scene*> m_scenes;
        bool                m_b_vsync          = true;
        Color               m_background_color = ColorDef::Pure::Gray;
    };
}
