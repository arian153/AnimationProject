#include "Track.hpp"

namespace CS460
{
    Track::Track()
    {
    }

    Track::~Track()
    {
    }

    void Track::Update(Real elapsed_time, KeyFrame& result)
    {
        SearchCurrentIDX(elapsed_time);
        if (current_idx == track_size - 1)
        {
            result.to_parent = key_frames[current_idx].to_parent;
        }
        else
        {
            size_t i = current_idx;
            size_t j = current_idx + 1;
            Interpolate(elapsed_time, key_frames[i], key_frames[j], result);
        }
    }

    void Track::Interpolate(Real t, const KeyFrame& start, const KeyFrame& end, KeyFrame& result) const
    {
        if (t <= start.time)
        {
            result = start;
            return;
        }
        if (t >= end.time)
        {
            result = end;
            return;
        }

        Real t1 = start.time;
        Real t2 = end.time;

        //Normalize the distance between the two keyframes
        Real seg_len          = t2 - t1;
        Real seg_start        = t - t1;
        Real seg_normalized_t = seg_start / seg_len;

        result.to_parent = Interpolation(start.to_parent, end.to_parent, seg_normalized_t);
        result.to_parent.rotation.SetNormalize();
    }

    void Track::SearchCurrentIDX(Real elapsed_time)
    {
        size_t last_idx = track_size - 1;
        //Search Forward in the keyframes for the interval
        while (current_idx != last_idx && key_frames[current_idx + 1].time < elapsed_time)
        {
            ++current_idx;
        }

        //Search Backward in the keyframes for the interval
        while (current_idx != 0 && key_frames[current_idx].time > elapsed_time)
        {
            --current_idx;
        }
    }
}
