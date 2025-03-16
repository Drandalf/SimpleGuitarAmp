// SimpleGuitarAmpEditor.cpp
#include "SimpleGuitarAmpEditor.h"

SimpleGuitarAmpEditor::SimpleGuitarAmpEditor(SimpleGuitarAmpAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set up the GUI components
    knobImage = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    // Gain slider
    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    gainSlider.setRange(1.0, 100.0, 0.1);
    gainSlider.setValue(audioProcessor.gainParam->get());
    addAndMakeVisible(gainSlider);

    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);

    // Tone slider
    toneSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    toneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    toneSlider.setRange(20.0, 20000.0, 1.0);
    toneSlider.setSkewFactorFromMidPoint(1000.0);
    toneSlider.setValue(audioProcessor.toneParam->get());
    addAndMakeVisible(toneSlider);

    toneLabel.setText("Tone", juce::dontSendNotification);
    toneLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(toneLabel);

    // Volume slider
    volumeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(audioProcessor.volumeParam->get());
    addAndMakeVisible(volumeSlider);

    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(volumeLabel);

    // Amp type combo box
    ampTypeComboBox.addItemList(audioProcessor.ampTypeParam->getAllValueStrings(), 1);
    ampTypeComboBox.setSelectedItemIndex(audioProcessor.ampTypeParam->getIndex(), juce::dontSendNotification);
    addAndMakeVisible(ampTypeComboBox);

    ampTypeLabel.setText("Amp Type", juce::dontSendNotification);
    ampTypeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(ampTypeLabel);

    // Connect sliders to parameters using lambdas
    gainSlider.onValueChange = [this]()
    {
        *audioProcessor.gainParam = gainSlider.getValue();
    };

    toneSlider.onValueChange = [this]()
    {
        *audioProcessor.toneParam = toneSlider.getValue();
    };

    volumeSlider.onValueChange = [this]()
    {
        *audioProcessor.volumeParam = volumeSlider.getValue();
    };

    ampTypeComboBox.onChange = [this]()
    {
        *audioProcessor.ampTypeParam = ampTypeComboBox.getSelectedItemIndex();
    };

    // Set the plugin's window size
    setSize(400, 300);
}

SimpleGuitarAmpEditor::~SimpleGuitarAmpEditor()
{
}

void SimpleGuitarAmpEditor::paint(juce::Graphics& g)
{
    // First draw the background color
    g.fillAll(juce::Colours::darkgrey);

    // Draw the background image if it's valid
    if (backgroundImage.isValid())
        g.drawImageAt(backgroundImage, 0, 0);
    
    // Draw a border around the plugin window
    g.setColour(juce::Colours::orange);
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(5.0f), 10.0f, 2.0f);
    
    // Draw the plugin name
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Simple Guitar Amp", getLocalBounds().reduced(10), juce::Justification::top, true);
}

void SimpleGuitarAmpEditor::resized()
{
    // Position the controls
    auto area = getLocalBounds().reduced(10);
    auto topSection = area.removeFromTop(40); // Space for title
    
    // Position sliders
    auto sliderWidth = area.getWidth() / 3;
    
    auto gainArea = area.removeFromLeft(sliderWidth);
    gainLabel.setBounds(gainArea.removeFromTop(20));
    gainSlider.setBounds(gainArea.reduced(10));
    
    auto toneArea = area.removeFromLeft(sliderWidth);
    toneLabel.setBounds(toneArea.removeFromTop(20));
    toneSlider.setBounds(toneArea.reduced(10));
    
    auto volumeArea = area.removeFromLeft(sliderWidth);
    volumeLabel.setBounds(volumeArea.removeFromTop(20));
    volumeSlider.setBounds(volumeArea.reduced(10));
    
    // Position amp type selector below the sliders
    auto comboBoxArea = getLocalBounds().reduced(10).removeFromBottom(60);
    ampTypeLabel.setBounds(comboBoxArea.removeFromTop(20));
    ampTypeComboBox.setBounds(comboBoxArea.reduced(sliderWidth / 2, 0));
}