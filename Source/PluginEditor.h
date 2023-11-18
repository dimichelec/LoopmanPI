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
    const int backgroundScale{ 2 };
    const int textBoxHeight{ 26 };

    // loop level knob
    LoopmanKnob loopLevelKnob;
    const Rectangle loopLevelKnobBounds{ Rectangle(378, 566, 166, 166 + textBoxHeight) / backgroundScale };
    ValueTree::SliderAttachment loopLevelAttachment;

    // buttons
    Image buttonBlankImage;
    const Rectangle buttonBlankBounds{ Rectangle(307, 830, 144, 108) };

    LoopmanMomentaryButton loopButton, stopButton, undoButton, redoButton, fadeoutButton;
    const Rectangle loopButtonBounds    { Rectangle(385, 777, 148, 110) / backgroundScale };
    const Rectangle stopButtonBounds    { Rectangle(388, 949, 148, 110) / backgroundScale };
    const Rectangle fadeoutButtonBounds { Rectangle(595, 949, 148, 110) / backgroundScale };
    const Rectangle undoButtonBounds    { Rectangle( 81, 901,  98,  98) / backgroundScale };
    const Rectangle redoButtonBounds    { Rectangle(206, 901,  98,  98) / backgroundScale };

    ValueTree::ButtonAttachment
        loopButtonAttachment, stopButtonAttachment, fadeoutButtonAttachment,
        undoButtonAttachment, redoButtonAttachment;

    // loop position widget
    LoopPosition loopPosition;
    const Rectangle loopPositionBounds{ Rectangle( 62, 622, 257, 257) / backgroundScale };

    // memory bar widget
    LoopmanMemoryBar memorySlider;
    const Rectangle memoryBarBounds{      Rectangle(601, 632, 134, 252) / backgroundScale };
    const Rectangle memoryBarTrackBounds{ Rectangle(655, 636,  28, 196) / backgroundScale };

    // timer parameters
    const int timerMilliseconds { 25 };
    const int timerRepaintCounts { 4 };
    int timerCount { 0 };

    void updateLoopControls();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopmanPIAudioProcessorEditor)
};
