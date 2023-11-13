/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LoopmanPIAudioProcessor::LoopmanPIAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
    , state (*this, nullptr, "STATE", {
        std::make_unique<juce::AudioParameterFloat>("loopLevel", "Loop Level",  0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterBool> ("loopButton","Loop Button", false),
        std::make_unique<juce::AudioParameterBool> ("stopButton","Stop Button", false),
        std::make_unique<juce::AudioParameterBool> ("undoButton","Undo Button", false),
        std::make_unique<juce::AudioParameterBool> ("redoButton","Redo Button", false)
    })
{
}

LoopmanPIAudioProcessor::~LoopmanPIAudioProcessor()
{
}

//==============================================================================
const juce::String LoopmanPIAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LoopmanPIAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LoopmanPIAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LoopmanPIAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LoopmanPIAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LoopmanPIAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LoopmanPIAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LoopmanPIAudioProcessor::setCurrentProgram (int) // index)
{

}

const juce::String LoopmanPIAudioProcessor::getProgramName (int) // index)
{
    return {};
}

void LoopmanPIAudioProcessor::changeProgramName (int, const juce::String&) // index, newName)
{

}

//==============================================================================
void LoopmanPIAudioProcessor::prepareToPlay (double sampleRate, int) // samplesPerBlock)
{
    looper.prepare(sampleRate);
}

void LoopmanPIAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LoopmanPIAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
#endif

void LoopmanPIAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) // midiMessages)
{
    // Sync w/ host
    // if (auto* playHead = getPlayHead())
    // {
    //   juce::AudioPlayHead::CurrentPositionInfo info;
    //   playHead->getCurrentPosition(info);
    // }

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // get control settings
    auto loopLevel = state.getParameter("loopLevel")->getValue();

    // process the loop buffer
    for (auto channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (auto i = 0; i < buffer.getNumSamples(); i++)
        {
            auto drySample = channelData[i];
            channelData[i] = drySample + (loopLevel * looper.getPlaySample(channel));
            looper.addLoopSample(channel, drySample);
        }
    }
}

//==============================================================================
bool LoopmanPIAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* LoopmanPIAudioProcessor::createEditor()
{
    return new LoopmanPIAudioProcessorEditor (*this);
}

//==============================================================================
void LoopmanPIAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xmlState = state.copyState().createXml())
        copyXmlToBinary(*xmlState, destData);
}

void LoopmanPIAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xmlState = getXmlFromBinary(data, sizeInBytes))
        state.replaceState(juce::ValueTree::fromXml(*xmlState));
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LoopmanPIAudioProcessor();
}
