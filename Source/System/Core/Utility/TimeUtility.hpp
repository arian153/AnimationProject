#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "CoreDef.hpp"
#include <chrono>

namespace CS460
{
    class TimeUtility
    {
    public:
        TimeUtility();
        ~TimeUtility();

        Real TotalTime() const;
        Real DeltaTime() const;

        void Reset();
        void Start();
        void Stop();
        void Tick();

        void RecordBegin();
        Real RecordEnd();
        

    private:
        //time data
        std::chrono::time_point<std::chrono::high_resolution_clock> m_base_point;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_prev_point;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_curr_point;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_stop_point;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_begin_point;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_end_point;

        //duration
        std::chrono::duration<R64> m_tick_duration;
        std::chrono::duration<R64> m_paused_duration;

        //others
        R64  m_delta_time = -1.0;
        bool m_b_stopped  = false;
    };
}
