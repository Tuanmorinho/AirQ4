#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>

class FilmstripButtonLook : public juce::LookAndFeel_V4 {
public:
  FilmstripButtonLook(juce::Image buttonStrip, int numFrames)
      : buttonImage(buttonStrip), frames(numFrames) {}

  void drawButtonBackground(juce::Graphics &g, juce::Button &button,
                            const juce::Colour &, bool shouldDrawButtonAsHighlighted,
                            bool shouldDrawButtonAsDown) override {
    const int frameHeight = buttonImage.getHeight() / frames;
    const int frameWidth = buttonImage.getWidth();

    // Use toggle state for frame selection: 0 = OFF, 1 = ON
    const int frameIndex = button.getToggleState() ? 1 : 0;

    auto bounds = button.getLocalBounds();
    g.drawImage(buttonImage, bounds.getX(), bounds.getY(), bounds.getWidth(),
                bounds.getHeight(), 0, frameIndex * frameHeight, frameWidth,
                frameHeight);
  }

  void drawButtonText(juce::Graphics &, juce::TextButton &, bool, bool) override {
    // No text needed for image button
  }

private:
  juce::Image buttonImage;
  int frames;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilmstripButtonLook)
};
