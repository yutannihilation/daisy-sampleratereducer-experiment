#include "daisysp.h"
#include "daisy_seed.h"
#include "reducer.h"

// clang-format off
#define OFFSET_SAMPLERATE_CONTROL 15
// clang-format on

using namespace daisysp;
using namespace daisy;
using namespace ns_reducer;

static DaisySeed hw;

AnalogControl samplerate_control;
Parameter samplerate_param;

static Reducer reducer;

static Metro metro;

uint8_t depth;
static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float sig;
    for (size_t i = 0; i < size; i += 2)
    {
        sig = in[i];
        reducer.SetFreq(fabsf(samplerate_param.Process()));
        sig = reducer.Process(sig);

        // left out
        out[i] = sig;

        // right out
        out[i + 1] = sig;
    }
}

int main(void)
{
    // initialize seed hardware and oscillator daisysp module
    float sample_rate;
    depth = 1;
    hw.Configure();
    hw.Init();
    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();

    // ADC
    AdcChannelConfig adcConfig[1];
    adcConfig[0].InitSingle(hw.GetPin(OFFSET_SAMPLERATE_CONTROL));
    hw.adc.Init(adcConfig, 1);

    reducer.Init();
    samplerate_control.Init(hw.adc.GetPtr(0), sample_rate);
    samplerate_param.Init(samplerate_control, 0.0, 1.0, Parameter::LINEAR);

    metro.Init(1.0f, sample_rate);

    // start callback
    hw.adc.Start();
    hw.StartAudio(AudioCallback);

    while (1)
    {
    }
}
