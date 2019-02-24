/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPlayerPluginAudioProcessor::AudioPlayerPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), params(*this, nullptr, "Parameters", std::make_unique<AudioParameterBool> ("Play", "Play", false))
#endif
{
    audioFormatManager.registerBasicFormats();
    audioFile.getSpecialLocation(File::SpecialLocationType::userHomeDirectory);
}

AudioPlayerPluginAudioProcessor::~AudioPlayerPluginAudioProcessor()
{
}

//==============================================================================
const String AudioPlayerPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPlayerPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPlayerPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPlayerPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPlayerPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPlayerPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPlayerPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPlayerPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String AudioPlayerPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioPlayerPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void AudioPlayerPluginAudioProcessor::openFile()
{
    FileChooser fileChooser { "Choose an audio file", audioFile, "" };
    
    if (fileChooser.browseForFileToOpen())
    {
        File choice = fileChooser.getResult();
        loadFile(choice);
    }
}

void AudioPlayerPluginAudioProcessor::loadFile(File& file)
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    audioFileSource = nullptr;
    
    AudioFormatReader* reader = audioFormatManager.createReaderFor(file);
    
    if (reader != nullptr)
    {
        audioFileSource = std::make_unique<AudioFormatReaderSource> (reader, true);
        transportSource.setSource(audioFileSource.get());
    }
}

//==============================================================================
void AudioPlayerPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    transportSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void AudioPlayerPluginAudioProcessor::releaseResources()
{
    transportSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioPlayerPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AudioPlayerPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    if (params.getParameterAsValue("Play") == true)
    {
        if (audioFileSource != nullptr)
        {
            transportSource.start();
        }
    }
    else
    {
        transportSource.stop();
        transportSource.setPosition(0.0);
    }
    
    
    transportSource.getNextAudioBlock(AudioSourceChannelInfo(buffer));
}

//==============================================================================
bool AudioPlayerPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AudioPlayerPluginAudioProcessor::createEditor()
{
    return new AudioPlayerPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPlayerPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioPlayerPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPlayerPluginAudioProcessor();
}
