// SimpleGuitarAmpEditor.h
#pragma once

#include "SimpleGuitarAmp.h"

class SimpleGuitarAmpEditor : public juce::AudioProcessorEditor
{
public:
    SimpleGuitarAmpEditor(SimpleGuitarAmpAudioProcessor&);
    ~SimpleGuitarAmpEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SimpleGuitarAmpAudioProcessor& audioProcessor;

    // Image resources
    juce::Image knobImage;
    juce::Image backgroundImage;

    // GUI components
    juce::Slider gainSlider;
    juce::Slider toneSlider;
    juce::Slider volumeSlider;
    juce::ComboBox ampTypeComboBox;

    juce::Label gainLabel;
    juce::Label toneLabel;
    juce::Label volumeLabel;
    juce::Label ampTypeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleGuitarAmpEditor)
};