#include <juce_dsp/juce_dsp.h>
#include <cmath>

class SoftSaturator
{
public:
    SoftSaturator() = default;

    void prepare(double sampleRate, int samplesPerBlock)
    {
        currentSampleRate = sampleRate;

        oversampler.reset();
        oversampler.initProcessing(samplesPerBlock);
        
        driveParam.reset(sampleRate, 0.02);
        mixParam.reset(sampleRate, 0.02);

        driveParam.setCurrentAndTargetValue(1.0f);
        mixParam.setCurrentAndTargetValue(1.0f);
    }

    void setDrive(float drive) { driveParam.setTargetValue(drive); }
    void setMix(float mix) { mixParam.setTargetValue(mix); }

    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        auto block = juce::dsp::AudioBlock<float>(buffer);

        // upsample
        auto oversampledBlock = oversampler.processSamplesUp(block);

        // asymmetrical soft clip (tube-like)
        for (int ch = 0; ch < oversampledBlock.getNumChannels(); ++ch)
        {
            auto* data = oversampledBlock.getChannelPointer(ch);
            for (int n = 0; n < oversampledBlock.getNumSamples(); ++n)
            {
                float drive = driveParam.getNextValue();
                float x = data[n];
                float bias = 0.2f; // lệch để sinh even harmonics
                data[n] = std::tanh(drive * (x + bias)) - std::tanh(drive * bias);
            }
        }

        // downsample
        oversampler.processSamplesDown(block);

        // wet/dry mix
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            auto* out = buffer.getWritePointer(ch);
            auto* in  = buffer.getReadPointer(ch);
            for (int n = 0; n < buffer.getNumSamples(); ++n)
            {
                float mix = mixParam.getNextValue();
                out[n] = mix * out[n] + (1.0f - mix) * in[n];
            }
        }
    }

private:
    double currentSampleRate = 48000.0;

    juce::dsp::Oversampling<float> oversampler{ 2, 4, juce::dsp::Oversampling<float>::filterHalfBandFIREquiripple, true };

    juce::SmoothedValue<float> driveParam;
    juce::SmoothedValue<float> mixParam;
};