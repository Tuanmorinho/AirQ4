#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>

class RealisticKnobLook : public juce::LookAndFeel_V4
{
public:
    RealisticKnobLook(juce::Image knobStrip, int numFrames)
        : knobImage(knobStrip), frames(numFrames) {}

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider& slider) override;

private:
    juce::Image knobImage;
    int frames;
};