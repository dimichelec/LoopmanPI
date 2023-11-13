/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Looper.h"
#include "LoopmanKnob.h"
#include "LoopPosition.h"
#include "LoopmanMemoryBar.h"


//==============================================================================
/**
*/
class LoopmanPIAudioProcessorEditor : public juce::AudioProcessorEditor,
    public juce::Button::Listener,
    public juce::Timer
{
public:
    LoopmanPIAudioProcessorEditor(LoopmanPIAudioProcessor&);
    ~LoopmanPIAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void timerCallback() override;

private:
    LoopmanPIAudioProcessor& audioProcessor;
    Looper* looper = &audioProcessor.looper;

    juce::Image backgroundImage, buttonBlankImage;
    const int backgroundScale{ 2 };
    const int textBoxHeight{ 26 };

    LoopmanKnob loopLevelKnob;
    const juce::Rectangle<int> loopLevelKnobBounds{ juce::Rectangle<int>(378, 566, 166, 166 + textBoxHeight) / backgroundScale };
    juce::AudioProcessorValueTreeState::SliderAttachment loopLevelAttachment;

    const juce::Rectangle<int> buttonBlankBounds{ juce::Rectangle<int>(307, 830, 144, 108) };

    juce::ImageButton loopButton, stopButton, undoButton, redoButton;
    const juce::Rectangle<int> loopButtonBounds{ juce::Rectangle<int>(390, 933, 148, 110) / backgroundScale };
    const juce::Rectangle<int> stopButtonBounds{ juce::Rectangle<int>(563, 933, 148, 110) / backgroundScale };
    const juce::Rectangle<int> undoButtonBounds{ juce::Rectangle<int>( 81, 901,  98,  98) / backgroundScale };
    const juce::Rectangle<int> redoButtonBounds{ juce::Rectangle<int>(206, 901,  98,  98) / backgroundScale };

    juce::AudioProcessorValueTreeState::ButtonAttachment
        loopButtonAttachment, stopButtonAttachment, undoButtonAttachment, redoButtonAttachment;

    LoopPosition loopPosition;
    const juce::Rectangle<int> loopPositionBounds{ juce::Rectangle<int>( 62, 622, 257, 257) / backgroundScale };

    LoopmanMemoryBar memorySlider;
    const juce::Rectangle<int> memoryBarBounds{      juce::Rectangle<int>(601, 632, 134, 252) / backgroundScale };
    const juce::Rectangle<int> memoryBarTrackBounds{ juce::Rectangle<int>(655, 636,  28, 196) / backgroundScale };

    const int timerMilliseconds { 25 };
    const int timerRepaintCounts { 4 };
    int timerCount { 0 };

    void updateLoopControls();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopmanPIAudioProcessorEditor)
};
