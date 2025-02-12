// include the SC Plugin API from the SC source files
#include <SC_PlugIn.hpp>
// include cstdio to have the printf function available
#include <cstdio>

//  the UGen ExampleMixer is implemented as a class which derives from SCUnit
class ExampleMixer : public SCUnit
{
public:
    // constructor of a UGen instance
    ExampleMixer() {
        // We set the actual function to calculate the next samples during runtime by using
        // setting the `mCalcFunc` member variable to a method using the `make_calc_function` template.
        // This allows to dynamically change the calc function based on given inputs.

        // the way to check if the UGen is requested at audio rate is to check if the output buffer size,
        // the number of samples we need to output within one block size, is > 1 (as control rate
        // is defined by returning 1 sample per block size).
        if (bufferSize() > 1 && isAudioRateIn(0) && isAudioRateIn(1)) {
            // dynamically define the function which will be called to consume and gerenate samples
            mCalcFunc = make_calc_function<ExampleMixer, &ExampleMixer::next>();

            // we need to "prime the pipes" of UGens that follow us in the graph,
            // therefore we calculate one sample.
            next(1);
        } else
        {
            if (!(isControlRateIn(0) && isControlRateIn(1))) {
                printf("Inputs of ExampleMixer are not using the same rate! Fall back to control rate.\n");
            }
            // the same for control rate, but using the method `next_k`
            mCalcFunc = make_calc_function<ExampleMixer, &ExampleMixer::next_k>();
            next_k(1);
        };
    }

    // method to calculate the next samples at control rate.
    // This method does not return a value but sets the value of our outputs.
    void next_k(int numSamples)
    {
        // in0(i) returns the first sample of the i-th input signal as a float number value
        const float signalA = in0(0);
        const float signalB = in0(1);

        // out0(i) sets the float value of the i-th output of our UGen
        out0(0) = (signalA + signalB) / 2.0f;
    }

    // method to calculate the next samples at audio rate
    void next(int numSamples) {
        // in(i) returns the first address of the i-th input signal which is as a pointer to a float number
        const float* signalA = in(0);
        const float* signalB = in(1);

        // out0(i) sets the float value of the i-th output.
        // As we want to modify its values, we don't declare it const.
        float* outBuffer = out(0);

        // we now iterate over the consecutive addresses of our outBuffer
        for(int i = 0; i < numSamples; ++i) {
            // we set the i-th sample of our output
            outBuffer[i] = (signalA[i] + signalB[i]) / 2.0f;
        };
    }
};

// the InterfaceTable stores all available UGens,
// so it is necessary to register the ExampleMixer in it as well.
// The PluginLoad macro will provide the actual interface.
PluginLoad(OSCMessengerUGens) {
    // register our UGen under the name ExampleMixer with the provided
    // interface table "inTable" from the PluginLoad macro.
    registerUnit<ExampleMixer>(inTable, "ExampleMixer", false);
}
