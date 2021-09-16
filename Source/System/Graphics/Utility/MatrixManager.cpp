#include "MatrixManager.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Utility/MatrixUtility.hpp"
#include "../Element/Scene.hpp"

namespace CS460
{
    MatrixManager::MatrixManager()
    {
    }

    MatrixManager::~MatrixManager()
    {
    }

    void MatrixManager::SetScreenAspect(Real screen_aspect)
    {
        m_screen_aspect      = screen_aspect;
        Real   client_height = m_top - m_bottom;
        Real   width         = screen_aspect * client_height;
        size_t client_width  = static_cast<size_t>(width);
        Real   half_x        = static_cast<Real>(client_width >> 1);
        m_right              = half_x;
        m_left               = -half_x;
        Update();
    }

    void MatrixManager::SetScreenAspect(size_t client_width, size_t client_height)
    {
        m_screen_aspect = (Real)client_width / (Real)client_height;
        Real half_x     = static_cast<Real>(client_width >> 1);
        Real half_y     = static_cast<Real>(client_height >> 1);
        m_right         = half_x;
        m_left          = -half_x;
        m_top           = half_y;
        m_bottom        = -half_y;
        Update();
    }

    void MatrixManager::SetFieldOfView(Real field_of_view)
    {
        m_field_of_view = field_of_view;
        Update();
    }

    void MatrixManager::SetFarPlane(Real far_plane)
    {
        m_far_plane = far_plane;
        Update();
    }

    void MatrixManager::SetNearPlane(Real near_plane)
    {
        m_near_plane = near_plane;
        Update();
    }

    void MatrixManager::SetClientWidth(size_t client_width)
    {
        Real half_x     = static_cast<Real>(client_width >> 1);
        m_right         = half_x;
        m_left          = -half_x;
        m_screen_aspect = (Real)client_width / (m_top - m_bottom);
        Update();
    }

    void MatrixManager::SetClientHeight(size_t client_height)
    {
        Real half_y     = static_cast<Real>(client_height >> 1);
        m_top           = half_y;
        m_bottom        = -half_y;
        m_screen_aspect = (m_right - m_left) / (Real)client_height;
        Update();
    }

    void MatrixManager::SetClientRect(size_t client_width, size_t client_height)
    {
        Real half_x     = static_cast<Real>(client_width >> 1);
        Real half_y     = static_cast<Real>(client_height >> 1);
        m_right         = half_x;
        m_left          = -half_x;
        m_top           = half_y;
        m_bottom        = -half_y;
        m_screen_aspect = (Real)client_width / (Real)client_height;
        Update();
    }

    void MatrixManager::SetClientRect(Real right, Real left, Real top, Real bottom)
    {
        m_right         = right;
        m_left          = left;
        m_top           = top;
        m_bottom        = bottom;
        m_screen_aspect = (m_right - m_left) / (m_top - m_bottom);
        Update();
    }

    Matrix44 MatrixManager::GetPerspectiveMatrix() const
    {
        return Math::Matrix44::PerspectiveMatrix(m_screen_aspect, m_field_of_view, m_far_plane, m_near_plane);
    }

    Matrix44 MatrixManager::GetOrthoGraphicMatrix() const
    {
        return Math::Matrix44::OrthoGraphicCenterMatrix(m_right, m_left, m_top, m_bottom, m_far_plane, m_near_plane);
    }

    Real MatrixManager::GetScreenRight() const
    {
        return m_right;
    }

    Real MatrixManager::GetScreenLeft() const
    {
        return m_left;
    }

    Real MatrixManager::GetScreenTop() const
    {
        return m_top;
    }

    Real MatrixManager::GetScreenBottom() const
    {
        return m_bottom;
    }

    Real MatrixManager::GetFarPlane() const
    {
        return m_far_plane;
    }

    Real MatrixManager::GetNearPlane() const
    {
        return m_near_plane;
    }

    Real MatrixManager::GetAspectRatio() const
    {
        return m_screen_aspect;
    }

    U32 MatrixManager::GetScreenWidth() const
    {
        return static_cast<U32>(m_right - m_left);
    }

    U32 MatrixManager::GetScreenHeight() const
    {
        return static_cast<U32>(m_top - m_bottom);
    }

    void MatrixManager::AddScene(Scene* scene)
    {
        m_scenes.push_back(scene);
    }

    void MatrixManager::RemoveScene(Scene* scene)
    {
        m_scenes.erase(std::find(m_scenes.begin(), m_scenes.end(), scene));
    }

    void MatrixManager::Update()
    {
        for (auto& scene : m_scenes)
        {
            scene->UpdateProjection();
        }
    }
}
