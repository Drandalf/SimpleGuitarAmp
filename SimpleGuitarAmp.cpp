// SimpleGuitarAmp.cpp
#include "SimpleGuitarAmp.h"
#include "SimpleGuitarAmpEditor.h"  // Add this include to fix the error

// Add these static distortion functions outside the class
static float cleanDistortionFunc(float x) { return juce::jlimit(-0.8f, 0.8f, x); }
static float crunchDistortionFunc(float x) { return std::tanh(x * 2.0f); }
static float leadDistortionFunc(float x) { return std::tanh(x * 4.0f); }
static float metalDistortionFunc(float x) { 
    constexpr float threshold = 0.3f;
    if (x > threshold)
        return threshold + (1.0f - threshold) * std::tanh((x - threshold) / (1.0f - threshold));
    else if (x < -threshold)
        return -threshold + (1.0f - threshold) * std::tanh((x + threshold) / (1.0f - threshold));
    else
        return x;
}

SimpleGuitarAmpAudioProcessor::SimpleGuitarAmpAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    // Add parameters
    addParameter(gainParam = new juce::AudioParameterFloat("gain", "Gain", 1.0f, 100.0f, 10.0f));
    addParameter(toneParam = new juce::AudioParameterFloat("tone", "Tone", 20.0f, 20000.0f, 1000.0f));
    addParameter(volumeParam = new juce::AudioParameterFloat("volume", "Volume", 0.0f, 1.0f, 0.5f));
    
    juce::StringArray ampTypes;
    ampTypes.add("Clean");
    ampTypes.add("Crunch");
    ampTypes.add("Lead");
    ampTypes.add("Metal");
    addParameter(ampTypeParam = new juce::AudioParameterChoice("ampType", "Amp Type", ampTypes, 0));
}

SimpleGuitarAmpAudioProcessor::~SimpleGuitarAmpAudioProcessor() {}

void SimpleGuitarAmpAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Prepare DSP chain
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());
    
    inputGain.prepare(spec);
    inputGain.setGainLinear(mapGain(gainParam->get()));
    
    distortion.prepare(spec);
    updateDistortionFunction(ampTypeParam->getIndex());
    
    toneFilter.prepare(spec);
    toneFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, toneParam->get());
    
    outputVolume.prepare(spec);
    outputVolume.setGainLinear(volumeParam->get());
}

void SimpleGuitarAmpAudioProcessor::releaseResources() {}

void SimpleGuitarAmpAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    
    // Update parameters
    inputGain.setGainLinear(mapGain(gainParam->get()));
    updateDistortionFunction(ampTypeParam->getIndex());
    toneFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), toneParam->get());
    outputVolume.setGainLinear(volumeParam->get());
    
    // Create audio block
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    
    // Process DSP chain
    inputGain.process(context);
    distortion.process(context);
    toneFilter.process(context);
    outputVolume.process(context);
}

void SimpleGuitarAmpAudioProcessor::processBlock(juce::AudioBuffer<double>& /*buffer*/, juce::MidiBuffer& /*midiMessages*/)
{
    // Not implemented - we just need this to avoid hiding the base class method
}

float SimpleGuitarAmpAudioProcessor::mapGain(float paramValue)
{
    // Map 1-100 to a reasonable gain range (0.1 to 10)
    return juce::jmap(paramValue, 1.0f, 100.0f, 0.1f, 10.0f);
}

void SimpleGuitarAmpAudioProcessor::updateDistortionFunction(int ampType)
{
    switch (ampType)
    {
        case 0: // Clean
            distortion.functionToUse = cleanDistortionFunc;
            break;
            
        case 1: // Crunch
            distortion.functionToUse = crunchDistortionFunc;
            break;
            
        case 2: // Lead
            distortion.functionToUse = leadDistortionFunc;
            break;
            
        case 3: // Metal
            distortion.functionToUse = metalDistortionFunc;
            break;
            
        default:
            distortion.functionToUse = cleanDistortionFunc;
            break;
    }
}

juce::AudioProcessorEditor* SimpleGuitarAmpAudioProcessor::createEditor()
{
    // Use your custom editor instead of the generic one
    return new SimpleGuitarAmpEditor(*this);
    // If you want to use the generic editor instead, uncomment this:
    // return new juce::GenericAudioProcessorEditor(*this);
}

bool SimpleGuitarAmpAudioProcessor::hasEditor() const
{
    return true;
}

const juce::String SimpleGuitarAmpAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleGuitarAmpAudioProcessor::acceptsMidi() const
{
    return false;
}

bool SimpleGuitarAmpAudioProcessor::producesMidi() const
{
    return false;
}

bool SimpleGuitarAmpAudioProcessor::isMidiEffect() const
{
    return false;
}

double SimpleGuitarAmpAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleGuitarAmpAudioProcessor::getNumPrograms()
{
    return 1;
}

int SimpleGuitarAmpAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleGuitarAmpAudioProcessor::setCurrentProgram(int /*index*/)
{
    // Not implemented for this simple plugin
}

const juce::String SimpleGuitarAmpAudioProcessor::getProgramName(int /*index*/)
{
    return {};
}

void SimpleGuitarAmpAudioProcessor::changeProgramName(int /*index*/, const juce::String& /*newName*/)
{
    // Not implemented for this simple plugin
}

void SimpleGuitarAmpAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Store parameters in memory block for saving preset
    juce::MemoryOutputStream stream(destData, true);
    stream.writeFloat(*gainParam);
    stream.writeFloat(*toneParam);
    stream.writeFloat(*volumeParam);
    stream.writeInt(ampTypeParam->getIndex());
}

void SimpleGuitarAmpAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Restore parameters from memory block (loading preset)
    juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
    if (stream.getNumBytesRemaining() >= 12)
    {
        *gainParam = stream.readFloat();
        *toneParam = stream.readFloat();
        *volumeParam = stream.readFloat();
        *ampTypeParam = stream.readInt();
    }
}

// This creates the plugin instance
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleGuitarAmpAudioProcessor();
}