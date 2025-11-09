#pragma once
#include <JuceHeader.h>

class LEDButtonLook : public juce::LookAndFeel_V4
{
public:
    LEDButtonLook(juce::Image ledStrip, int numFrames)
        : ledImage(ledStrip), frames(numFrames) {}

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, 
                            const juce::Colour& backgroundColour,
                            bool shouldDrawButtonAsHighlighted, 
                            bool shouldDrawButtonAsDown) override
    {
        if (!ledImage.isValid())
            return;

        const int frameHeight = ledImage.getHeight() / frames;
        const int frameWidth = ledImage.getWidth();
        
        // Frame 0 = OFF, Frame 1 = ON
        const int frameIndex = button.getToggleState() ? 1 : 0;

        g.drawImage(ledImage,
                    button.getLocalBounds().toFloat(),
                    0, frameIndex * frameHeight, frameWidth, frameHeight);
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                       bool shouldDrawButtonAsHighlighted, 
                       bool shouldDrawButtonAsDown) override
    {
        // Không vẽ text - chỉ hiển thị LED image
    }

private:
    juce::Image ledImage;
    int frames;
};