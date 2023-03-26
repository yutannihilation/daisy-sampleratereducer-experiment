#include "daisysp.h"
#include "reducer.h"
#include <math.h>

using namespace ns_reducer;
void Reducer::Init()
{
    frequency_ = .2f;
    phase_ = 0.0f;
    sample_ = 0.0f;
    next_sample_ = 0.0f;
    previous_sample_ = 0.0f;
}

float Reducer::Process(float in)
{
    float this_sample = next_sample_;
    next_sample_ = 0.f;
    phase_ += frequency_;
    if (phase_ >= 1.0f)
    {
        phase_ -= 1.0f;
        float t = phase_ / frequency_;
        // t = 0: the transition occurred right at this sample.
        // t = 1: the transition occurred at the previous sample.
        // Use linear interpolation to recover the fractional sample.
        float new_sample = previous_sample_ + (in - previous_sample_) * (1.0f - t);
        float discontinuity = new_sample - sample_;
        this_sample += discontinuity * daisysp::ThisBlepSample(t);
        next_sample_ = discontinuity * daisysp::NextBlepSample(t);
        sample_ = new_sample;
    }
    next_sample_ += sample_;
    previous_sample_ = in;

    return this_sample;
}

void Reducer::SetFreq(float frequency)
{
    frequency_ = daisysp::fclamp(frequency, 0.f, 1.f);
}
