#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>

// Simple ON/OFF indicator light (not audio signal)
class SignalIndicator : public juce::Component {
public:
  SignalIndicator(juce::Image indicatorStrip, int numFrames)
      : indicatorImage(indicatorStrip), frames(numFrames), isOn(false) {}

  void paint(juce::Graphics &g) override {
    const int frameHeight = indicatorImage.getHeight() / frames;
    const int frameWidth = indicatorImage.getWidth();

    // Frame selection: 0 = OFF, 1 = ON
    const int frameIndex = isOn ? 1 : 0;

    auto bounds = getLocalBounds();
    g.drawImage(indicatorImage, bounds.getX(), bounds.getY(),
                bounds.getWidth(), bounds.getHeight(), 0,
                frameIndex * frameHeight, frameWidth, frameHeight);
  }

  void setOn(bool on) {
    if (isOn != on) {
      isOn = on;
      repaint();
    }
  }

  bool getState() const { return isOn; }

private:
  juce::Image indicatorImage;
  int frames;
  bool isOn;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SignalIndicator)
};
