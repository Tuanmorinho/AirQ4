#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>

// Audio level indicator with opacity based on signal strength
class AudioLevelIndicator : public juce::Component, private juce::Timer {
public:
  AudioLevelIndicator(juce::Image signalStrip, int numFrames)
      : signalImage(signalStrip), frames(numFrames), currentLevel(-60.0f) {
    startTimerHz(30); // Update at 30Hz for smooth decay
  }

  void paint(juce::Graphics &g) override {
    const int frameHeight = signalImage.getHeight() / frames;
    const int frameWidth = signalImage.getWidth();
    auto bounds = getLocalBounds();

    if (currentLevel <= -60.0f) {
      // No signal: show frame 0 (OFF)
      g.drawImage(signalImage, bounds.getX(), bounds.getY(),
                  bounds.getWidth(), bounds.getHeight(), 0, 0, frameWidth,
                  frameHeight);
    } else {
      // Has signal: show frame 1 (ON) with opacity 30% to 100%
      float opacity = calculateOpacity(currentLevel);
      g.setOpacity(opacity);
      g.drawImage(signalImage, bounds.getX(), bounds.getY(),
                  bounds.getWidth(), bounds.getHeight(), 0, frameHeight,
                  frameWidth, frameHeight);
    }
  }

  void setLevel(float levelInDb) {
    currentLevel = levelInDb;
    repaint();
  }

private:
  void timerCallback() override {
    // Smooth decay (optional - can be removed if not needed)
  }

  float calculateOpacity(float levelDb) const {
    // Map -60dB to 0dB → 30% to 100% opacity
    const float minDb = -60.0f;
    const float maxDb = 0.0f;
    const float minOpacity = 0.3f;
    const float maxOpacity = 1.0f;

    if (levelDb <= minDb)
      return minOpacity;
    if (levelDb >= maxDb)
      return maxOpacity;

    // Linear interpolation from 30% to 100%
    float normalized = (levelDb - minDb) / (maxDb - minDb);
    return minOpacity + normalized * (maxOpacity - minOpacity);
  }

  juce::Image signalImage;
  int frames;
  float currentLevel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioLevelIndicator)
};
