#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "RealisticKnobLook.h"
#include "EQGraphComponent.h"
#include "PluginProcessor.h"

class AirQ4AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    AirQ4AudioProcessorEditor(AirQ4AudioProcessor&);
    ~AirQ4AudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AirQ4AudioProcessor& processor;
    
    RealisticKnobLook realisticLook;

    juce::Image backgroundImage;

    juce::Slider sliderSubLowCut;
    juce::Slider sliderLowShelf40Hz;
    juce::Slider sliderBell160Hz;
    juce::Slider sliderBell650Hz;
    juce::Slider sliderBell2k5Hz;
    juce::Slider sliderHighShelfAir;
    juce::Slider sliderAirFreq;

    void setupRotarySlider(juce::Slider& slider, double min, double max, double init);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AirQ4AudioProcessorEditor)
};
