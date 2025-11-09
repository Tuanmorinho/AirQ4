#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "modules/gui/RealisticKnobLook.h"
#include "modules/gui/LEDButtonLook.h"
#include "EQGraphComponent.h"
#include "PluginProcessor.h"
#include "BinaryData.h"

class AirQ4AudioProcessorEditor : public juce::AudioProcessorEditor,
                                 private juce::Timer
{
public:
    AirQ4AudioProcessorEditor(AirQ4AudioProcessor&);
    ~AirQ4AudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AirQ4AudioProcessor& processor;
    
    // LookAndFeel cho từng loại knob
    std::unique_ptr<RealisticKnobLook> subKnobLook;
    std::unique_ptr<RealisticKnobLook> knob40Look;
    std::unique_ptr<RealisticKnobLook> knob160Look;
    std::unique_ptr<RealisticKnobLook> knob650Look;
    std::unique_ptr<RealisticKnobLook> knob2k5Look;
    std::unique_ptr<RealisticKnobLook> airKnobLook;
    std::unique_ptr<RealisticKnobLook> airGainLook;
    
    // LookAndFeel cho LED buttons
    std::unique_ptr<LEDButtonLook> onOffLEDLook;
    std::unique_ptr<LEDButtonLook> peakLEDLook;

    // Background image
    juce::Image backgroundImage;

    // Sliders
    juce::Slider sliderSubLowCut;
    juce::Slider sliderLowShelf40Hz;
    juce::Slider sliderBell160Hz;
    juce::Slider sliderBell650Hz;
    juce::Slider sliderBell2k5Hz;
    juce::Slider sliderHighShelfAir;
    juce::Slider sliderAirFreq;

    // Labels
    juce::Label labelSubLowCut;
    juce::Label labelLowShelf40Hz;
    juce::Label labelBell160Hz;
    juce::Label labelBell650Hz;
    juce::Label labelBell2k5Hz;
    juce::Label labelHighShelfAir;
    juce::Label labelAirFreq;
    
    // Additional controls (từ UI design)
    juce::ToggleButton btnOnOff;
    juce::ToggleButton btnPeakSignal;  // Read-only indicator
    
    // Peak detection timer
    void checkPeakLevel();

    void setupRotarySlider(juce::Slider& slider);
    void loadAssets();
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AirQ4AudioProcessorEditor)
};