#include "FrameUtility.hpp"

namespace CS460
{
    FrameUtility::FrameUtility()
    {
    }

    FrameUtility::~FrameUtility()
    {
    }

    void FrameUtility::CalculateFrameStatus(Real total_time)
    {
        m_frame_count++;
        // Compute averages over one second period.
        if (total_time - m_elapsed_time >= 1.0f)
        {
            m_curr_fps  = (Real)m_frame_count; // fps = frame count / 1 second
            m_curr_mspf = 1000.0f / m_curr_fps;
            // Reset for next average.
            m_frame_count = 0;
            m_elapsed_time += 1.0f;
        }
    }

    Real FrameUtility::GetFramePerSecond() const
    {
        return m_curr_fps;
    }

    Real FrameUtility::GetMillisecondPerFrame() const
    {
        return m_curr_mspf;
    }

    Real FrameUtility::GetSecondPerFrame() const
    {
        return (1.0f / m_curr_fps);
    }
}
