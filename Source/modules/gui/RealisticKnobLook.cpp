#include "RealisticKnobLook.h"

void RealisticKnobLook::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                                          juce::Slider& slider)
{
    if (knobImage.isValid())
    {
        const int frameHeight = knobImage.getHeight() / frames;
        const int frameWidth  = knobImage.getWidth();
        const int frameIndex  = juce::jlimit(0, frames - 1, (int)(sliderPosProportional * (float)(frames - 1)));

        g.drawImage(knobImage,
                    x, y, width, height,
                    0, frameIndex * frameHeight, frameWidth, frameHeight);
    }
}
