#include "PluginEditor.h"
#include "modules/dsp/dsp_helper.h"
#include <BinaryData.h>

AirQ4AudioProcessorEditor::AirQ4AudioProcessorEditor(AirQ4AudioProcessor &p)
    : AudioProcessorEditor(&p), processor(p) {
  // Load background image from BinaryData
  backgroundImage = juce::ImageCache::getFromMemory(
      BinaryData::background_png, BinaryData::background_pngSize);

  // Load knob filmstrip images and create LookAndFeel instances
  // Main knobs: 192x4032 = 21 frames (192x192 each)
  const int numFrames = 21;
  const int numFramesAirFreq = 5; // air_knot_strip: 193x961

  auto subKnobImage = juce::ImageCache::getFromMemory(
      BinaryData::sub_knob_strip_png, BinaryData::sub_knob_strip_pngSize);
  lookSubKnob = std::make_unique<RealisticKnobLook>(subKnobImage, numFrames);

  auto knob40Image = juce::ImageCache::getFromMemory(
      BinaryData::_40_knob_strip_png, BinaryData::_40_knob_strip_pngSize);
  look40Knob = std::make_unique<RealisticKnobLook>(knob40Image, numFrames);

  auto knob160Image = juce::ImageCache::getFromMemory(
      BinaryData::_160_knob_strip_png, BinaryData::_160_knob_strip_pngSize);
  look160Knob = std::make_unique<RealisticKnobLook>(knob160Image, numFrames);

  auto knob650Image = juce::ImageCache::getFromMemory(
      BinaryData::_650_knob_strip_png, BinaryData::_650_knob_strip_pngSize);
  look650Knob = std::make_unique<RealisticKnobLook>(knob650Image, numFrames);

  auto knob2k5Image = juce::ImageCache::getFromMemory(
      BinaryData::_2k5_knob_strip_png, BinaryData::_2k5_knob_strip_pngSize);
  look2k5Knob = std::make_unique<RealisticKnobLook>(knob2k5Image, numFrames);

  auto airGainImage =
      juce::ImageCache::getFromMemory(BinaryData::air_gain_knob_strip_png,
                                      BinaryData::air_gain_knob_strip_pngSize);
  lookAirGainKnob =
      std::make_unique<RealisticKnobLook>(airGainImage, numFrames);

  auto airFreqImage = juce::ImageCache::getFromMemory(
      BinaryData::air_knot_strip_png, BinaryData::air_knot_strip_pngSize);
  lookAirFreqKnob =
      std::make_unique<RealisticKnobLook>(airFreqImage, numFramesAirFreq);

  // Setup all sliders
  setupRotarySlider(sliderSubLowCut);
  sliderSubLowCut.setLookAndFeel(lookSubKnob.get());

  setupRotarySlider(sliderLowShelf40Hz);
  sliderLowShelf40Hz.setLookAndFeel(look40Knob.get());

  setupRotarySlider(sliderBell160Hz);
  sliderBell160Hz.setLookAndFeel(look160Knob.get());

  setupRotarySlider(sliderBell650Hz);
  sliderBell650Hz.setLookAndFeel(look650Knob.get());

  setupRotarySlider(sliderBell2k5Hz);
  sliderBell2k5Hz.setLookAndFeel(look2k5Knob.get());

  setupRotarySlider(sliderAirGain);
  sliderAirGain.setLookAndFeel(lookAirGainKnob.get());

  setupRotarySlider(sliderHighShelfAir);
  sliderHighShelfAir.setRange(0.0, 1.0, 0.25); // Override: 5 positions for air freq
  sliderHighShelfAir.setLookAndFeel(lookAirFreqKnob.get());

  // Setup slider callbacks to update processor values
  sliderSubLowCut.onValueChange = [this]() {
    processor.sliderSubLowCut = (float)sliderSubLowCut.getValue();
  };

  sliderLowShelf40Hz.onValueChange = [this]() {
    processor.sliderLowShelf40Hz = (float)sliderLowShelf40Hz.getValue();
  };

  sliderBell160Hz.onValueChange = [this]() {
    processor.sliderBell160Hz = (float)sliderBell160Hz.getValue();
  };

  sliderBell650Hz.onValueChange = [this]() {
    processor.sliderBell650Hz = (float)sliderBell650Hz.getValue();
  };

  sliderBell2k5Hz.onValueChange = [this]() {
    processor.sliderBell2k5Hz = (float)sliderBell2k5Hz.getValue();
  };

  sliderAirGain.onValueChange = [this]() {
    processor.sliderAirGain = (float)sliderAirGain.getValue();
  };

  sliderHighShelfAir.onValueChange = [this]() {
    // Map 0-1 slider to 0-4 index (5 positions)
    processor.selectedAirFreqIndex =
        juce::jlimit(0, 4, (int)(sliderHighShelfAir.getValue() * 4.0f + 0.5f));
  };

  // Set initial slider values from processor
  sliderSubLowCut.setValue(processor.sliderSubLowCut,
                           juce::dontSendNotification);
  sliderLowShelf40Hz.setValue(processor.sliderLowShelf40Hz,
                              juce::dontSendNotification);
  sliderBell160Hz.setValue(processor.sliderBell160Hz,
                           juce::dontSendNotification);
  sliderBell650Hz.setValue(processor.sliderBell650Hz,
                           juce::dontSendNotification);
  sliderBell2k5Hz.setValue(processor.sliderBell2k5Hz,
                           juce::dontSendNotification);
  sliderAirGain.setValue(processor.sliderAirGain, juce::dontSendNotification);
  // Map index 0-4 to slider value 0.0-1.0
  sliderHighShelfAir.setValue(processor.selectedAirFreqIndex / 4.0f,
                              juce::dontSendNotification);

  // Set editor size to 1x (half of 2x background: 1601x401)
  setSize(801, 200);
}

void AirQ4AudioProcessorEditor::paint(juce::Graphics &g) {
  // Draw background image scaled to 1x (background is 2x: 1601x401)
  if (backgroundImage.isValid()) {
    g.drawImage(backgroundImage, getLocalBounds().toFloat(),
                juce::RectanglePlacement::stretchToFit);
  } else {
    // Fallback background
    g.fillAll(juce::Colours::darkgrey);
  }
}

void AirQ4AudioProcessorEditor::resized() {
  // Background 2x: 1601 x 401 pixels, displayed at 1x: 800 x 200
  // Main knobs 2x: 192x192, displayed at 1x: 96x96
  const int knobSize = 96;    // 192 / 2
  const int airKnobSize = 96; // 193 / 2 (rounded)

  // All positions scaled to 1x (divided by 2)
  // Top row
  sliderSubLowCut.setBounds(61.9, 26, knobSize, knobSize);
  sliderBell160Hz.setBounds(238, 26, knobSize, knobSize);
  sliderBell2k5Hz.setBounds(415, 26, knobSize, knobSize);
  sliderHighShelfAir.setBounds(597.5, 25.5, airKnobSize, airKnobSize);

  // Bottom row
  sliderLowShelf40Hz.setBounds(149, 91.5, knobSize, knobSize);
  sliderBell650Hz.setBounds(326, 91.5, knobSize, knobSize);
  sliderAirGain.setBounds(503.5, 91.5, knobSize, knobSize);
}

void AirQ4AudioProcessorEditor::setupRotarySlider(juce::Slider &slider) {
  slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  slider.setRange(0.0, 1.0, 0.05); // 21 positions: 0.00, 0.05, 0.10, ..., 1.00
  slider.setValue(0.5); // Center position = 0dB
  slider.setDoubleClickReturnValue(true, 0.5);
  addAndMakeVisible(slider);
}
