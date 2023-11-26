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
#include "LoopmanMomentaryButton.h"
#include "LoopmanMemoryBar.h"

using Image = juce::Image;
using Rectangle = juce::Rectangle<int>;
using ValueTree = juce::AudioProcessorValueTreeState;

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

    // background and general control things
    Image backgroundImage;
    const int backgroundScale   { 2 };
    const int textBoxHeight     { 26 };

    // loop level knob
    LoopmanKnob loopLevelKnob, outputLevelKnob;
    const Rectangle loopLevelKnobBounds     { Rectangle(384, 136, 144, 144 + textBoxHeight) / backgroundScale };
    const Rectangle outputLevelKnobBounds   { Rectangle(575, 136, 144, 144 + textBoxHeight) / backgroundScale };

    ValueTree::SliderAttachment
        loopLevelAttachment, outputLevelAttachment;

    // buttons
    Image buttonBlankImage;

    LoopmanMomentaryButton loopButton, stopButton, undoButton, redoButton, fadeoutButton;
    const Rectangle loopButtonBounds    { Rectangle( 26, 434, 126,  92) / backgroundScale };
    const Rectangle stopButtonBounds    { Rectangle(180, 434, 126,  92) / backgroundScale };
    const Rectangle fadeoutButtonBounds { Rectangle(643, 434, 126,  92) / backgroundScale };
    const Rectangle undoButtonBounds    { Rectangle(334, 434, 126,  92) / backgroundScale };
    const Rectangle redoButtonBounds    { Rectangle(488, 434, 126,  92) / backgroundScale };

    ValueTree::ButtonAttachment
        loopButtonAttachment, stopButtonAttachment, fadeoutButtonAttachment,
        undoButtonAttachment, redoButtonAttachment;

    // loop position widget
    LoopPosition loopPosition;
    const Rectangle loopPositionBounds  { Rectangle( 45, 135, 272, 272) / backgroundScale };

    // memory bar widget
    LoopmanMemoryBar memorySlider;
    const Rectangle memoryBarBounds     { Rectangle(382, 326, 339,  67) / backgroundScale };
    const Rectangle memoryBarTrackBounds{ Rectangle(387, 330, 330,  28) / backgroundScale };

    // timer parameters
    const int timerMilliseconds { 25 };
    const int timerRepaintCounts { 4 };
    int timerCount { 0 };

    void updateLoopControls();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopmanPIAudioProcessorEditor)
};
