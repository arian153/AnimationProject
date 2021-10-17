#include "Track.hpp"

namespace CS460
{
    Track::Track()
    {
    }

    Track::~Track()
    {
    }

    void Track::Update(Real dt, KeyFrame& result)
    {
        size_t i = current_idx;
        size_t j = i < track_size - 1 ? i + 1 : 0;

        Interpolate(track_pos, key_frames[i], key_frames[j], result);

        track_pos += dt;
        if (track_pos >= 1.0f)
        {
            //reset track
            track_pos   = 0.0f;
            current_idx = 0;
        }
        else
        {
            if (track_pos >= key_frames[j].time)
            {
                for (size_t k = j; k < track_size - 1; ++k)
                {
                    if (key_frames[k + 1].time > track_pos)
                    {
                        current_idx = k;
                        break;
                    }
                }
            }
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

        result.to_parent = Interpolation(start.to_parent, end.to_parent, t);
        result.to_parent.rotation.SetNormalize();
    }
}
