#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

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
  float sliderSubLowCut = 0.0f;
  float sliderLowShelf40Hz = 0.0f;
  float sliderBell160Hz = 0.0f;
  float sliderBell650Hz = 0.0f;
  float sliderBell2k5Hz = 0.0f;
  float sliderAirGain = 0.0f;

  // Air band freq selector (0-4 corresponding to knob positions)
  // Index: 0=2.5kHz, 1=4kHz, 2=6kHz, 3=8kHz, 4=10kHz
  int selectedAirFreqIndex = 3; // Default: 8kHz (index 3)

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AirQ4AudioProcessor)
};
