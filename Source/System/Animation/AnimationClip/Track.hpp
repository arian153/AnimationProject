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
        void Update(Real elapsed_time, KeyFrame& result);
        void Interpolate(Real t, const KeyFrame& start, const KeyFrame& end, KeyFrame& result) const;
        void SetUpCurrentIDX(Real elapsed_time);

    public:
        size_t                current_idx = 0;
        size_t                track_size  = 0;
        std::vector<KeyFrame> key_frames;
    };
}
