#include "PluginEditor.h"
#include "modules/dsp/dsp_helper.h"

AirQ4AudioProcessorEditor::AirQ4AudioProcessorEditor(AirQ4AudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(800, 200);

    setupRotarySlider(sliderSubLowCut);
    setupRotarySlider(sliderLowShelf40Hz);
    setupRotarySlider(sliderBell160Hz);
    setupRotarySlider(sliderBell650Hz);
    setupRotarySlider(sliderBell2k5Hz);
    setupRotarySlider(sliderHighShelfAir);

    sliderSubLowCut.onValueChange = [this]()
    {
        processor.sliderSubLowCut = (float)sliderSubLowCut.getValue();
    };

    sliderLowShelf40Hz.onValueChange = [this]()
    {
        processor.sliderLowShelf40Hz = (float)sliderLowShelf40Hz.getValue();
    };

    sliderBell160Hz.onValueChange = [this]()
    {
        processor.sliderBell160Hz = (float)sliderBell160Hz.getValue();
    };

    sliderBell650Hz.onValueChange = [this]()
    {
        processor.sliderBell650Hz = (float)sliderBell650Hz.getValue();
    };

    sliderBell2k5Hz.onValueChange = [this]()
    {
        processor.sliderBell2k5Hz = (float)sliderBell2k5Hz.getValue();
    };

    sliderHighShelfAir.onValueChange = [this]()
    {
        processor.sliderHighShelfAir = (float)sliderHighShelfAir.getValue();
    };

    sliderAirFreq.onValueChange = [this]()
    {
        processor.selectedAirFreq = (float)sliderAirFreq.getValue();
    };
}

void AirQ4AudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void AirQ4AudioProcessorEditor::resized()
{
    int sliderWidth = 80;
    int sliderHeight = 200;
    int padding = 20;
    int x = padding;
    int y = 50;

    sliderSubLowCut.setBounds(x, y, sliderWidth, sliderHeight); labelSubLowCut.setBounds(x, y-20, sliderWidth, 20);
    x += sliderWidth + padding;

    sliderLowShelf40Hz.setBounds(x, y, sliderWidth, sliderHeight); labelLowShelf40Hz.setBounds(x, y-20, sliderWidth, 20);
    x += sliderWidth + padding;

    sliderBell160Hz.setBounds(x, y, sliderWidth, sliderHeight); labelBell160Hz.setBounds(x, y-20, sliderWidth, 20);
    x += sliderWidth + padding;

    sliderBell650Hz.setBounds(x, y, sliderWidth, sliderHeight); labelBell650Hz.setBounds(x, y-20, sliderWidth, 20);
    x += sliderWidth + padding;

    sliderBell2k5Hz.setBounds(x, y, sliderWidth, sliderHeight); labelBell2k5Hz.setBounds(x, y-20, sliderWidth, 20);
    x += sliderWidth + padding;

    sliderHighShelfAir.setBounds(x, y, sliderWidth, sliderHeight); labelHighShelfAir.setBounds(x, y-20, sliderWidth, 20);
    comboAirFreq.setBounds(x, y + sliderHeight + 10, sliderWidth, 30); labelAirFreq.setBounds(x, y + sliderHeight + 40, sliderWidth, 20);

}

void AirQ4AudioProcessorEditor::setupRotarySlider(juce::Slider& slider, juce::Label& label, juce::Label& valueLabel, const juce::String& name)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    slider.setRange(-5.0, 5.0, 0.1);
    slider.setValue(0.0);

    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(slider);
    addAndMakeVisible(label);
    addAndMakeVisible(valueLabel);
}