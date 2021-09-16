#pragma once
#include "OSAPI.hpp"
#include IncludeOSAPI
#include "WindowMode.hpp"

namespace CS460
{
    class LevelManager;
    class TimeUtility;

    class OSCommon : public OSAPI
    {
    public:
        explicit OSCommon(Application* application);
        ~OSCommon();

        void Initialize();
        void Shutdown();

        void SetConfineCursor(bool b_confine_cursor);
        void SetShowCursor(bool b_show_cursor);
        void SetWindowMode(eWindowMode window_mode);
        void SetQuit(bool b_quit);
        void SetClientResolution(int width, int height);
        void SetMonitorResolution();
        void DispatchMessagePump() const;

        int  ClientWidth() const;
        int  ClientHeight() const;
        bool IsFullscreen() const;
        bool IsConfineCursor() const;
        bool IsShowCursor() const;
        bool IsInit() const;
        bool IsQuit() const;
        bool IsPaused() const;
        Real AspectRatio() const;

        eWindowMode WindowMode() const;

        void SetLevelManager(LevelManager* level_manager);
    private :
        void OnResize() const;
        void OnFullscreen() const;
        void DispatchPaused() const;
        void DispatchActive() const;

    private:
        friend class OSAPI;

    private:
        bool m_b_loaded = false;
        bool m_b_init   = false;

        //mouse option
        bool m_b_show_cursor    = true;
        bool m_b_confine_cursor = false;

        //window style option
        eWindowMode  m_window_mode         = eWindowMode::Windowed;
        std::wstring m_application_caption = L"Engine 5th";
        bool         m_b_resizable         = false;

        //window application state
        bool m_b_app_paused = false;
        bool m_b_minimized  = false;
        bool m_b_maximized  = false;
        bool m_b_quit       = false;

        //application window size
        int m_curr_client_width  = 1280;
        int m_curr_client_height = 720;
        int m_prev_client_width  = 1280;
        int m_prev_client_height = 720;

        //actual monitor resolution
        int m_monitor_screen_width  = 0;
        int m_monitor_screen_height = 0;

        TimeUtility*  m_application_timer = nullptr;
        LevelManager* m_level_manager     = nullptr;
    };
}
