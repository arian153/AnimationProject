#pragma once
#include <vector>

#include "KeyFrame.hpp"

namespace CS460
{
    class Track
    {
    public:
        Track();
        ~Track();
        void Update(Real dt, KeyFrame& result);

        void Interpolate(Real t, const KeyFrame& start, const KeyFrame& end, KeyFrame& result) const;

    public:
        size_t                current_idx = 0;
        size_t                track_size  = 0;
        Real                  track_pos   = 0.0f;
        Real                  duration    = 0.0f;
        std::vector<KeyFrame> key_frames;
    };
}
