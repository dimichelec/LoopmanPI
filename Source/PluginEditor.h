/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Looper.h"

//==============================================================================
/**
*/
class LoopmanPIAudioProcessorEditor   : public juce::AudioProcessorEditor,
                                        public juce::Button::Listener,
                                        public juce::Timer
{
public:
    LoopmanPIAudioProcessorEditor (LoopmanPIAudioProcessor&);
    ~LoopmanPIAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void timerCallback() override;

private:
    LoopmanPIAudioProcessor& audioProcessor;
    Looper* looper = &audioProcessor.looper;

    juce::Slider gainSlider;
    juce::Label gainLabel;

    juce::Slider loopLevelKnob;
    juce::Label loopLevelLabel;

    juce::AudioProcessorValueTreeState::SliderAttachment
        gainAttachment,
        loopLevelAttachment;

    juce::TextButton loopButton, stopButton;
    juce::AudioProcessorValueTreeState::ButtonAttachment
        loopButtonAttachment,
        stopButtonAttachment;

    juce::Label playStateLabel;
    juce::Component loopPosition;

    double memoryUsage { looper->getUsage() };
    juce::ProgressBar memoryUsageBar { memoryUsage };

    const int timerMilliseconds { 25 };
    const int timerRepaintCounts { 4 };
    int timerCount { 0 };

    void updateLoopControls();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopmanPIAudioProcessorEditor)
};
