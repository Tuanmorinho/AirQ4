    labelAirFreq.setText("FREQ", juce::dontSendNotification);
    labelAirFreq.setJustificationType(juce::Justification::centred);
    labelAirFreq.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(labelAirFreq);

    // Setup brand labels
    labelAirQ4.setText("AirQ4", juce::dontSendNotification);
    labelAirQ4.setFont(juce::Font(32.0f, juce::Font::bold));
    label#include "PluginEditor.h"
#include "modules/dsp/dsp_helper.h"

AirQ4AudioProcessorEditor::AirQ4AudioProcessorEditor(AirQ4AudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Load assets
    loadAssets();
    
    // Start timer for peak detection (30Hz refresh)
    startTimerHz(30);

    // Setup sliders với LookAndFeel tương ứng
    setupRotarySlider(sliderSubLowCut);
    sliderSubLowCut.setLookAndFeel(subKnobLook.get());
    
    setupRotarySlider(sliderLowShelf40Hz);
    sliderLowShelf40Hz.setLookAndFeel(knob40Look.get());
    
    setupRotarySlider(sliderBell160Hz);
    sliderBell160Hz.setLookAndFeel(knob160Look.get());
    
    setupRotarySlider(sliderBell650Hz);
    sliderBell650Hz.setLookAndFeel(knob650Look.get());
    
    setupRotarySlider(sliderBell2k5Hz);
    sliderBell2k5Hz.setLookAndFeel(knob2k5Look.get());
    
    // Air Gain: 0 to 10
    setupRotarySlider(sliderHighShelfAir);
    sliderHighShelfAir.setRange(0.0, 10.0, 0.1);
    sliderHighShelfAir.setLookAndFeel(airGainLook.get());
    
    // Air Freq selector: discrete values (0=OFF, 1=2.5k, 2=5k, 3=8k, 4=10k)
    setupRotarySlider(sliderAirFreq);
    sliderAirFreq.setRange(0.0, 4.0, 1.0); // 5 positions
    sliderAirFreq.setValue(3.0); // default = 8kHz
    sliderAirFreq.setTextValueSuffix("");
    sliderAirFreq.textFromValueFunction = [](double value) -> juce::String
    {
        int pos = (int)value;
        switch (pos)
        {
            case 0: return "OFF";
            case 1: return "2.5k";
            case 2: return "5k";
            case 3: return "8k";
            case 4: return "10k";
            default: return "8k";
        }
    };
    sliderAirFreq.setLookAndFeel(airKnobLook.get());

    // Setup labels
    labelSubLowCut.setText("SUB", juce::dontSendNotification);
    labelSubLowCut.setJustificationType(juce::Justification::centred);
    labelSubLowCut.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(labelSubLowCut);

    labelLowShelf40Hz.setText("40Hz", juce::dontSendNotification);
    labelLowShelf40Hz.setJustificationType(juce::Justification::centred);
    labelLowShelf40Hz.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(labelLowShelf40Hz);

    labelBell160Hz.setText("160Hz", juce::dontSendNotification);
    labelBell160Hz.setJustificationType(juce::Justification::centred);
    labelBell160Hz.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(labelBell160Hz);

    labelBell650Hz.setText("650Hz", juce::dontSendNotification);
    labelBell650Hz.setJustificationType(juce::Justification::centred);
    labelBell650Hz.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(labelBell650Hz);

    labelBell2k5Hz.setText("2.5kHz", juce::dontSendNotification);
    labelBell2k5Hz.setJustificationType(juce::Justification::centred);
    labelBell2k5Hz.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(labelBell2k5Hz);

    labelHighShelfAir.setText("AIR", juce::dontSendNotification);
    labelHighShelfAir.setJustificationType(juce::Justification::centred);
    labelHighShelfAir.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(labelHighShelfAir);

    labelAirFreq.setText("FREQ", juce::dontSendNotification);
    labelAirFreq.setJustificationType(juce::Justification::centred);
    labelAirFreq.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(labelAirFreq);

    // Callback cho sliders (giá trị -5 đến 5 trực tiếp)
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
        // Map discrete positions to actual frequencies
        int position = (int)sliderAirFreq.getValue();
        switch (position)
        {
            case 0: processor.selectedAirFreq = 0.0f;      break; // OFF
            case 1: processor.selectedAirFreq = 2500.0f;   break; // 2.5 kHz
            case 2: processor.selectedAirFreq = 5000.0f;   break; // 5 kHz
            case 3: processor.selectedAirFreq = 8000.0f;   break; // 8 kHz
            case 4: processor.selectedAirFreq = 10000.0f;  break; // 10 kHz
            default: processor.selectedAirFreq = 8000.0f;
        }
    };

    // Setup ON/OFF button
    btnOnOff.setToggleState(true, juce::dontSendNotification);
    btnOnOff.setClickingTogglesState(true);
    btnOnOff.setLookAndFeel(onOffLEDLook.get());
    btnOnOff.onClick = [this]()
    {
        processor.isBypassed = !btnOnOff.getToggleState();
    };
    addAndMakeVisible(btnOnOff);

    // Setup PEAK indicator (read-only)
    btnPeakSignal.setToggleState(false, juce::dontSendNotification);
    btnPeakSignal.setClickingTogglesState(false); // Không cho click
    btnPeakSignal.setLookAndFeel(peakLEDLook.get());
    addAndMakeVisible(btnPeakSignal);

    // Set kích thước của editor
    setSize(800, 400);
}

void AirQ4AudioProcessorEditor::loadAssets()
{
    // Load background
    backgroundImage = juce::ImageCache::getFromMemory(
        BinaryData::background_png, 
        BinaryData::background_pngSize
    );

    // Load knob strips (giả sử mỗi strip có 101 frames cho -5 đến 5)
    auto subKnobStrip = juce::ImageCache::getFromMemory(
        BinaryData::sub_knob_strip_png, 
        BinaryData::sub_knob_strip_pngSize
    );
    subKnobLook = std::make_unique<RealisticKnobLook>(subKnobStrip, 101);

    auto knob40Strip = juce::ImageCache::getFromMemory(
        BinaryData::_40_knob_strip_png, 
        BinaryData::_40_knob_strip_pngSize
    );
    knob40Look = std::make_unique<RealisticKnobLook>(knob40Strip, 101);

    auto knob160Strip = juce::ImageCache::getFromMemory(
        BinaryData::_160_knob_strip_png, 
        BinaryData::_160_knob_strip_pngSize
    );
    knob160Look = std::make_unique<RealisticKnobLook>(knob160Strip, 101);

    auto knob650Strip = juce::ImageCache::getFromMemory(
        BinaryData::_650_knob_strip_png, 
        BinaryData::_650_knob_strip_pngSize
    );
    knob650Look = std::make_unique<RealisticKnobLook>(knob650Strip, 101);

    auto knob2k5Strip = juce::ImageCache::getFromMemory(
        BinaryData::_2k5_knob_strip_png, 
        BinaryData::_2k5_knob_strip_pngSize
    );
    knob2k5Look = std::make_unique<RealisticKnobLook>(knob2k5Strip, 101);

    auto airGainStrip = juce::ImageCache::getFromMemory(
        BinaryData::air_gain_strip_png, 
        BinaryData::air_gain_strip_pngSize
    );
    airGainLook = std::make_unique<RealisticKnobLook>(airGainStrip, 101);

    auto airKnobStrip = juce::ImageCache::getFromMemory(
        BinaryData::air_knob_strip_png, 
        BinaryData::air_knob_strip_pngSize
    );
    airKnobLook = std::make_unique<RealisticKnobLook>(airKnobStrip, 101);

    // Load LED strips (2 frames: OFF/ON)
    auto onOffLEDStrip = juce::ImageCache::getFromMemory(
        BinaryData::signal_on_off_strip_png,
        BinaryData::signal_on_off_strip_pngSize
    );
    onOffLEDLook = std::make_unique<LEDButtonLook>(onOffLEDStrip, 2);

    auto peakLEDStrip = juce::ImageCache::getFromMemory(
        BinaryData::signal_peak_strip_png,
        BinaryData::signal_peak_strip_pngSize
    );
    peakLEDLook = std::make_unique<LEDButtonLook>(peakLEDStrip, 2);
}

void AirQ4AudioProcessorEditor::paint(juce::Graphics& g)
{
    // Vẽ background
    if (backgroundImage.isValid())
    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat());
    }
    else
    {
        g.fillAll(juce::Colour(0xff2a2a2a)); // fallback color
    }
}

void AirQ4AudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Dựa vào thiết kế: width ~1400px, height ~400px ratio
    // Main knobs row
    int knobSize = 80;
    int smallKnobSize = 50;
    int startY = 120;
    int labelOffsetY = -35;
    
    // Positions (measured from design)
    int x1 = 80;   // Q4 (SUB)
    int x2 = 240;  // 40Hz bottom row
    int x3 = 320;  // 160Hz
    int x4 = 480;  // 40Hz top row (SUB button above)
    int x5 = 640;  // 650Hz
    int x6 = 800;  // 2.5kHz
    int x7 = 960;  // AIR GAIN
    int x8 = 1100; // AIR BAND selector
    
    // Top row labels
    labelAirQ4.setBounds(20, 30, 150, 40);
    labelTAudio.setBounds(bounds.getWidth() - 200, 30, 180, 40);
    
    // Left side: ON button + SUB knob
    btnOnOff.setBounds(60, 200, 60, 60);
    sliderSubLowCut.setBounds(x1, startY, knobSize, knobSize);
    labelSubLowCut.setBounds(x1, startY + labelOffsetY, knobSize, 25);
    
    // Second column: 40Hz (LOW SHELF)
    sliderLowShelf40Hz.setBounds(x2, startY + 60, knobSize, knobSize);
    labelLowShelf40Hz.setBounds(x2, startY + 60 + labelOffsetY, knobSize, 25);
    // Small SUB button above 40Hz
    
    // 160Hz
    sliderBell160Hz.setBounds(x3, startY, knobSize, knobSize);
    labelBell160Hz.setBounds(x3, startY + labelOffsetY, knobSize, 25);
    
    // 650Hz (bottom row)
    sliderBell650Hz.setBounds(x5, startY + 60, knobSize, knobSize);
    labelBell650Hz.setBounds(x5, startY + 60 + labelOffsetY, knobSize, 25);
    
    // 2.5kHz (top row)
    sliderBell2k5Hz.setBounds(x6, startY, knobSize, knobSize);
    labelBell2k5Hz.setBounds(x6, startY + labelOffsetY, knobSize, 25);
    
    // AIR GAIN (bottom row, large yellow knob)
    sliderHighShelfAir.setBounds(x7, startY + 60, knobSize, knobSize);
    labelHighShelfAir.setBounds(x7, startY + 60 + labelOffsetY, knobSize, 25);
    
    // AIR BAND selector (top right, blue knob)
    sliderAirFreq.setBounds(x8, startY - 10, 70, 70);
    labelAirFreq.setBounds(x8 - 10, startY - 45, 90, 25);
    
    // Right side: PEAK indicator + SIGNAL button
    btnPeakSignal.setBounds(bounds.getWidth() - 100, 200, 60, 60);
}


void AirQ4AudioProcessorEditor::setupRotarySlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0); // Ẩn text box
    slider.setRange(-5.0, 5.0, 0.1);
    slider.setValue(0.0);
    slider.setDoubleClickReturnValue(true, 0.0); // Double-click để reset về 0
    addAndMakeVisible(slider);
}

void AirQ4AudioProcessorEditor::timerCallback()
{
    // Check peak level từ processor
    bool isPeaking = processor.currentPeakLevel > 0.95f; // threshold -0.5dB
    btnPeakSignal.setToggleState(isPeaking, juce::dontSendNotification);
}