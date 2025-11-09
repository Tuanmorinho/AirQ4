#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <functional>

class EQGraphComponent : public juce::Component,
                         private juce::Timer
{
public:
    EQGraphComponent()
    {
        startTimerHz(30); // cập nhật đồ thị 30Hz
    }

    void setMappings(std::function<float(float)> qFunc,
                     std::function<float(float)> gainFunc)
    {
        qMapping = qFunc;
        gainMapping = gainFunc;
        repaint();
    }

    void setSliderValue(float value)
    {
        sliderValue = value;
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey);
        auto bounds = getLocalBounds().toFloat();

        // Khung
        g.setColour(juce::Colours::white);
        g.drawRect(bounds, 1.0f);

        // Đường Q
        if (qMapping)
        {
            g.setColour(juce::Colours::red);
            juce::Path qPath;
            qPath.startNewSubPath(bounds.getX(), bounds.getBottom() - qMapping(-5.0f)/qScale()*bounds.getHeight());
            for (float s = -5.0f; s <= 5.0f; s += 0.5f)
            {
                float x = juce::jmap(s, -5.0f, 5.0f, bounds.getX(), bounds.getRight());
                float y = bounds.getBottom() - qMapping(s)/qScale()*bounds.getHeight();
                qPath.lineTo(x, y);
            }
            g.strokePath(qPath, juce::PathStrokeType(2.0f));
        }

        // Đường Gain
        if (gainMapping)
        {
            g.setColour(juce::Colours::green);
            juce::Path gainPath;
            gainPath.startNewSubPath(bounds.getX(), bounds.getBottom() - gainMapping(-5.0f)/gainScale()*bounds.getHeight());
            for (float s = -5.0f; s <= 5.0f; s += 0.5f)
            {
                float x = juce::jmap(s, -5.0f, 5.0f, bounds.getX(), bounds.getRight());
                float y = bounds.getBottom() - gainMapping(s)/gainScale()*bounds.getHeight();
                gainPath.lineTo(x, y);
            }
            g.strokePath(gainPath, juce::PathStrokeType(2.0f));
        }

        // Vị trí slider
        g.setColour(juce::Colours::yellow);
        float sliderX = juce::jmap(sliderValue, -5.0f, 5.0f, bounds.getX(), bounds.getRight());
        g.drawLine(sliderX, bounds.getY(), sliderX, bounds.getBottom(), 2.0f);
    }

private:
    std::function<float(float)> qMapping;
    std::function<float(float)> gainMapping;
    float sliderValue = 0.0f;

    float qScale() { return 12.0f; }      // scale max Q
    float gainScale() { return 12.0f; }   // scale max Gain

    void timerCallback() override { repaint(); }
};
