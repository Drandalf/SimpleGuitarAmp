// SimpleGuitarAmp.h
#pragma once

#include <JuceHeader.h>

class SimpleGuitarAmpAudioProcessor : public juce::AudioProcessor
{
public:
    SimpleGuitarAmpAudioProcessor();
    ~SimpleGuitarAmpAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    // Add the double version to avoid hiding the base class method
    void processBlock(juce::AudioBuffer<double>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Parameters
    juce::AudioParameterFloat* gainParam;
    juce::AudioParameterFloat* toneParam;
    juce::AudioParameterFloat* volumeParam;
    juce::AudioParameterChoice* ampTypeParam;

private:
    // DSP components
    juce::dsp::Gain<float> inputGain;
    juce::dsp::WaveShaper<float> distortion;
    juce::dsp::IIR::Filter<float> toneFilter;
    juce::dsp::Gain<float> outputVolume;

    // Parameter mapping functions
    float mapGain(float paramValue);

    // Helper to set the appropriate distortion function based on amp type
    void updateDistortionFunction(int ampType);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleGuitarAmpAudioProcessor)
};