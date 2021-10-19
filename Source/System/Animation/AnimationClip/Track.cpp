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
        SetUpCurrentIDX(elapsed_time);
        //clamping last keyframe of track
        if (current_idx == track_size - 1)
        {
            result.to_parent = key_frames[current_idx].to_parent;
        }
        else
        {
            Interpolate(elapsed_time, key_frames[current_idx], key_frames[current_idx + 1], result);
        }
    }

    void Track::Interpolate(Real t, const KeyFrame& start, const KeyFrame& end, KeyFrame& result) const
    {
        //clamping simple case
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

        //normalize time between the two keyframes
        Real normalized_t = (t - start.time) / (end.time - start.time);

        result.to_parent = Interpolation(start.to_parent, end.to_parent, normalized_t);
        result.to_parent.rotation.SetNormalize();
    }

    void Track::SetUpCurrentIDX(Real elapsed_time)
    {
        //find current index for interpolation.
        size_t last_idx = track_size - 1;
        //search forward in the keyframes
        while (current_idx != last_idx && key_frames[current_idx + 1].time < elapsed_time)
        {
            ++current_idx;
        }

        //search backward in the keyframes
        while (current_idx != 0 && key_frames[current_idx].time > elapsed_time)
        {
            --current_idx;
        }
    }
}
