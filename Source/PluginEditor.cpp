/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPlayerPluginAudioProcessorEditor::AudioPlayerPluginAudioProcessorEditor (AudioPlayerPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    Timer::startTimerHz(60.0);
    
    playButton.setToggleState(false, NotificationType::dontSendNotification);
    playButton.setClickingTogglesState(true);
    addAndMakeVisible(playButton);
    
    buttonState = std::make_unique <AudioProcessorValueTreeState::ButtonAttachment> (processor.params, "Play", playButton);
    
    openButton.onClick = [this]() { processor.openFile(); };
    addAndMakeVisible(openButton);
    
    setSize (200, 200);
}

AudioPlayerPluginAudioProcessorEditor::~AudioPlayerPluginAudioProcessorEditor()
{
}

void AudioPlayerPluginAudioProcessorEditor::timerCallback()
{
    if (playButton.getToggleState() == false)
    {
        playButton.setColour(TextButton::buttonColourId, Colours::red);
        playButton.setButtonText("Stopped");
    }
    else
    {
        playButton.setColour(TextButton::buttonColourId, Colours::limegreen);
        playButton.setButtonText("Playing");
    }
}
//==============================================================================
void AudioPlayerPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::black);
}

void AudioPlayerPluginAudioProcessorEditor::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    FlexBox flexbox;
    
    flexbox.flexDirection = FlexBox::Direction::column;
    flexbox.alignContent = FlexBox::AlignContent::stretch;
    flexbox.alignItems = FlexBox::AlignItems::stretch;
    flexbox.justifyContent = FlexBox::JustifyContent::center;
    
    flexbox.items.add(FlexItem(50, 50, openButton));
    flexbox.items.add(FlexItem(50, 50, playButton));
    flexbox.performLayout(bounds);
}
