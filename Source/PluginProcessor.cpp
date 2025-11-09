#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "modules/dsp/dsp_helper.h"

AirQ4AudioProcessor::AirQ4AudioProcessor()
#if defined(AIRQ4_MONO)
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::mono(), true)
        .withOutput("Output", juce::AudioChannelSet::mono(), true))
#elif defined(AIRQ4_STEREO)
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
#else
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
#endif
{}
AirQ4AudioProcessor::~AirQ4AudioProcessor() {}

#if defined(AIRQ4_MONO)
const juce::String AirQ4AudioProcessor::getName() const { return "AirQ4 Mono"; }
#elif defined(AIRQ4_STEREO)
const juce::String AirQ4AudioProcessor::getName() const { return "AirQ4 Stereo"; }
#else
const juce::String AirQ4AudioProcessor::getName() const { return "AirQ4"; }
#endif

bool AirQ4AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AirQ4AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AirQ4AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AirQ4AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AirQ4AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AirQ4AudioProcessor::getCurrentProgram()
{
    return 0;
}

void AirQ4AudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AirQ4AudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AirQ4AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

void AirQ4AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumInputChannels());

    subLowCut25HzFilter.prepare(spec);
    lowShelf40HzFilter.prepare(spec);
    bell160HzFilter.prepare(spec);
    bell650HzFilter.prepare(spec);
    bell2k5HzFilter.prepare(spec);
    highShelfAirFilter.prepare(spec);

    subLowCut25HzFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, SUB_LOW_CUT_FREQ, SUB_Q_MIN);
    lowShelf40HzFilter.coefficients  = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, LOWSHELF_FREQ, LOWSHELF_Q_MIN, juce::Decibels::decibelsToGain(0.0f));
    bell160HzFilter.coefficients     = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, BELL1_FREQ, BELL_Q_MIN, juce::Decibels::decibelsToGain(0.0f));
    bell650HzFilter.coefficients     = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, BELL2_FREQ, BELL_Q_MIN, juce::Decibels::decibelsToGain(0.0f));
    bell2k5HzFilter.coefficients     = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, BELL3_FREQ, BELL_Q_MIN, juce::Decibels::decibelsToGain(0.0f));
    highShelfAirFilter.coefficients  = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, HIGHSHELF_AIR_FREQ, HIGHSHELF_Q_MIN, juce::Decibels::decibelsToGain(0.0f));
}

void AirQ4AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AirQ4AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void AirQ4AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{   
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> ctx(block);

    float Qsub = calcSubLowCutQ(sliderSubLowCut);
    subLowCut25HzFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), SUB_LOW_CUT_FREQ, Qsub);

    auto [gainLS, qLS] = calcLowShelf40Hz(sliderLowShelf40Hz);
    lowShelf40HzFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), LOWSHELF_FREQ, qLS, juce::Decibels::decibelsToGain(gainLS));

    auto [gain160, q160] = calcBell(sliderBell160Hz);
    bell160HzFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), BELL1_FREQ, q160, juce::Decibels::decibelsToGain(gain160));

    auto [gain650, q650] = calcBell(sliderBell650Hz);
    bell650HzFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), BELL2_FREQ, q650, juce::Decibels::decibelsToGain(gain650));

    auto [gain2k5, q2k5] = calcBell(sliderBell2k5Hz);
    bell2k5HzFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), BELL3_FREQ, q2k5, juce::Decibels::decibelsToGain(gain2k5));

    auto [gainHS, qHS] = calcHighShelfAir(sliderHighShelfAir);
    highShelfAirFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(getSampleRate(), selectedAirFreq, qHS, juce::Decibels::decibelsToGain(gainHS));

    subLowCut25HzFilter.process(ctx);
    lowShelf40HzFilter.process(ctx);
    bell160HzFilter.process(ctx);
    bell650HzFilter.process(ctx);
    bell2k5HzFilter.process(ctx);
    highShelfAirFilter.process(ctx);
}

bool AirQ4AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AirQ4AudioProcessor::createEditor()
{
    return new AirQ4AudioProcessorEditor(*this);
}

void AirQ4AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void AirQ4AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AirQ4AudioProcessor();
}
