#pragma once

#include "PluginProcessor.h"
#include "modules/gui/RealisticKnobLook.h"
#include "modules/gui/FilmstripButtonLook.h"
#include "modules/gui/SignalIndicator.h"
#include "modules/gui/AudioLevelIndicator.h"
#include "modules/gui/PeakIndicator.h"
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

class AirQ4AudioProcessorEditor : public juce::AudioProcessorEditor,
                                  private juce::Timer {
public:
  AirQ4AudioProcessorEditor(AirQ4AudioProcessor &);
  ~AirQ4AudioProcessorEditor() override = default;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  AirQ4AudioProcessor &processor;

  // Background image
  juce::Image backgroundImage;

  // LookAndFeel for each knob (using different filmstrips)
  std::unique_ptr<RealisticKnobLook> lookSubKnob;
  std::unique_ptr<RealisticKnobLook> look40Knob;
  std::unique_ptr<RealisticKnobLook> look160Knob;
  std::unique_ptr<RealisticKnobLook> look650Knob;
  std::unique_ptr<RealisticKnobLook> look2k5Knob;
  std::unique_ptr<RealisticKnobLook> lookAirGainKnob;
  std::unique_ptr<RealisticKnobLook> lookAirFreqKnob;

  // Sliders
  juce::Slider sliderSubLowCut;
  juce::Slider sliderLowShelf40Hz;
  juce::Slider sliderBell160Hz;
  juce::Slider sliderBell650Hz;
  juce::Slider sliderBell2k5Hz;
  juce::Slider sliderAirGain;
  juce::Slider sliderHighShelfAir;

  // On/Off button and indicators
  std::unique_ptr<FilmstripButtonLook> lookOnOffButton;
  juce::TextButton btnOnOff;
  std::unique_ptr<SignalIndicator> signalIndicator;
  std::unique_ptr<AudioLevelIndicator> audioLevelIndicator;
  std::unique_ptr<PeakIndicator> peakIndicator;

  void setupRotarySlider(juce::Slider &slider);
  void timerCallback() override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AirQ4AudioProcessorEditor)
};
