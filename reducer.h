#include <stdint.h>
#ifdef __cplusplus

/** @file reducer.h */

namespace ns_reducer
{
  /**

  Based on the DaisySP's implementation of SampleRateReducer[1], which is ported from
  the Mutable Instruments Plaits' code[2], written by Emilie Gillet in 2014.

  [1]: https://github.com/electro-smith/DaisySP/blob/master/Source/Effects/sampleratereducer.h
  [2]: https://github.com/pichenettes/eurorack/blob/master/plaits/dsp/fx/sample_rate_reducer.h

  */
  class Reducer
  {
  public:
    Reducer() {}
    ~Reducer() {}

    void Init();

    /** Get the next floating point sample
        \param in Sample to be processed.
    */
    float Process(float in);

    /** Set the new sample rate.
        \param frequency over 0-1. 1 is full quality, .5 is half sample rate, etc.
    */
    void SetFreq(float frequency);

  private:
    float frequency_;
    float phase_;
    float sample_;
    float previous_sample_;
    float next_sample_;
  };
} // namespace ns_reducer
#endif
