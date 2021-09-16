#pragma once
#include "../../Math/Algebra/Matrix44.hpp"
#include <vector>
#include "../../Core/Utility/CoreDef.hpp"

namespace CS460
{
    class Scene;

    class MatrixManager
    {
    public:
        MatrixManager();
        ~MatrixManager();

        //Setter
        void SetScreenAspect(Real screen_aspect);
        void SetScreenAspect(size_t client_width, size_t client_height);
        void SetFieldOfView(Real field_of_view);
        void SetFarPlane(Real far_plane);
        void SetNearPlane(Real near_plane);
        void SetClientWidth(size_t client_width);
        void SetClientHeight(size_t client_height);
        void SetClientRect(size_t client_width, size_t client_height);
        void SetClientRect(Real right, Real left, Real top, Real bottom);

        //Getter
        Matrix44 GetPerspectiveMatrix() const;
        Matrix44 GetOrthoGraphicMatrix() const;

        Real GetScreenRight() const;
        Real GetScreenLeft() const;
        Real GetScreenTop() const;
        Real GetScreenBottom() const;

        Real GetFarPlane() const;
        Real GetNearPlane() const;
        Real GetAspectRatio() const;

        U32 GetScreenWidth() const;
        U32 GetScreenHeight() const;

        void AddScene(Scene* scene);
        void RemoveScene(Scene* scene);
        void Update();
    private:
        Real m_right  = 0.0f;
        Real m_left   = 0.0f;
        Real m_top    = 0.0f;
        Real m_bottom = 0.0f;

        Real m_screen_aspect = 1.0f;
        Real m_field_of_view = Math::PI_DIV_4;

        Real m_far_plane  = 1000.f;
        Real m_near_plane = 0.001f;

        std::vector<Scene*> m_scenes;
    };
}
