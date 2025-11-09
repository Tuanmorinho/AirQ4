#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>

// Peak indicator: shows red when signal > 0dB
class PeakIndicator : public juce::Component, private juce::Timer {
public:
  PeakIndicator(juce::Image peakStrip, int numFrames)
      : peakImage(peakStrip), frames(numFrames), isPeaking(false),
        peakHoldTime(0) {
    startTimerHz(30); // Update at 30Hz for peak decay
  }

  void paint(juce::Graphics &g) override {
    const int frameHeight = peakImage.getHeight() / frames;
    const int frameWidth = peakImage.getWidth();
    auto bounds = getLocalBounds();

    // Frame 0 = OFF, Frame 1 = ON (red)
    const int frameIndex = isPeaking ? 1 : 0;

    g.drawImage(peakImage, bounds.getX(), bounds.getY(), bounds.getWidth(),
                bounds.getHeight(), 0, frameIndex * frameHeight, frameWidth,
                frameHeight);
  }

  void setLevel(float levelInDb) {
    if (levelInDb > 0.0f) {
      isPeaking = true;
      peakHoldTime = 1000; // Hold peak for 1 second (1000ms)
      repaint();
    }
  }

private:
  void timerCallback() override {
    // Decay peak hold
    if (isPeaking && peakHoldTime > 0) {
      peakHoldTime -= 33; // ~30Hz = 33ms per frame
      if (peakHoldTime <= 0) {
        isPeaking = false;
        repaint();
      }
    }
  }

  juce::Image peakImage;
  int frames;
  bool isPeaking;
  int peakHoldTime;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PeakIndicator)
};
