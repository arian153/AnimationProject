#include "AnimationClip.hpp"

namespace CS460
{
    AnimationClip::AnimationClip()
    {
    }

    AnimationClip::~AnimationClip()
    {
    }

    void AnimationClip::Update(Real dt)
    {
        KeyFrame result;
        size_t   i = current_idx;
        size_t   j = i < track_size - 1 ? i + 1 : 0;

        Interpolate(track_position, track[i], track[j], result);
        //find final result & matrix

        //do after interpolation calculation
        track_position += dt;

        if (track_position >= duration)
        {
            //reset track of animation clip
            track_position = 0.0f;
            current_idx    = 0;
        }
        else
        {
            if (track_position >= track[j].time)
            {
                for (size_t k = j; k < track_size - 1; ++k)
                {
                    if (track[k + 1].time > track_position)
                    {
                        current_idx = k;
                        break;
                    }
                }
            }
        }
    }

    void AnimationClip::Shutdown()
    {
        track.clear();
    }

    void AnimationClip::Interpolate(Real t, const KeyFrame& start, const KeyFrame& end, KeyFrame& result) const
    {
        size_t start_size = start.to_parents.size();
        size_t end_size   = end.to_parents.size();

        if (start_size == end_size)
        {
            result.to_parents.resize(start_size);
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
            for (size_t i = 0; i < end_size; ++i)
            {
                result.to_parents[i] = Interpolation(start.to_parents[i], end.to_parents[i], t);
            }
        }
        else
        {
            //bone size is not match. something problem.
            result.to_parents.resize(0);
        }
    }
}
