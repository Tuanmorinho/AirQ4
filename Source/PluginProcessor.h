#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "modules/dsp/soft_saturator.cpp"

constexpr float SUB_LOW_CUT_FREQ = 25.0f;
constexpr float LOWSHELF_FREQ = 40.0f;
constexpr float BELL1_FREQ = 160.0f;
constexpr float BELL2_FREQ = 650.0f;
constexpr float BELL3_FREQ = 2500.0f;
constexpr float HIGHSHELF_AIR_FREQ = 8000.0f;

class AirQ4AudioProcessor final : public juce::AudioProcessor {
public:
  AirQ4AudioProcessor();
  ~AirQ4AudioProcessor() override;

  // Core
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
  using AudioProcessor::processBlock;

  // Editor
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  // Info
  const juce::String getName() const override;
  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  // Programs
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int, const juce::String &) override;

  // State
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  // DSP filters
  juce::dsp::IIR::Filter<float> subLowCut25HzFilter;
  juce::dsp::IIR::Filter<float> lowShelf40HzFilter;
  juce::dsp::IIR::Filter<float> bell160HzFilter;
  juce::dsp::IIR::Filter<float> bell650HzFilter;
  juce::dsp::IIR::Filter<float> bell2k5HzFilter;
  juce::dsp::IIR::Filter<float> highShelfAirFilter;

  // Slider values 0→1
  float sliderSubLowCut = 0.5f;     // Default: center (0dB)
  float sliderLowShelf40Hz = 0.5f;  // Default: center (0dB)
  float sliderBell160Hz = 0.5f;     // Default: center (0dB)
  float sliderBell650Hz = 0.5f;     // Default: center (0dB)
  float sliderBell2k5Hz = 0.5f;     // Default: center (0dB)
  float sliderAirGain = 0.0f;       // Default: 0dB

  // Air band freq selector (0-4 corresponding to knob positions)
  // Index: 0=OFF, 1=4kHz, 2=6kHz, 3=8kHz, 4=10kHz
  int selectedAirFreqIndex = 0; // Default: OFF

  // Bypass state
  bool isBypassed = false; // Default: ON (not bypassed)

  // Audio level metering
  std::atomic<float> outputLevelDb{-60.0f}; // Output level in dB

  float getOutputLevel() const { return outputLevelDb.load(); }

  // Saturator parameters (0-1)
  float saturatorDrive = 1.5f; // Default: moderate drive
  float saturatorMix = 0.3f;   // Default: 30% wet

private:
  // Soft saturator for harmonic coloring
  SoftSaturator softSaturator;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AirQ4AudioProcessor)
};
