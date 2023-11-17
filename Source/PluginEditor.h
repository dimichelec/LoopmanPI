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


using Rectangle = juce::Rectangle<int>;

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
    const Rectangle loopLevelKnobBounds{ Rectangle(378, 566, 166, 166 + textBoxHeight) / backgroundScale };
    juce::AudioProcessorValueTreeState::SliderAttachment loopLevelAttachment;

    const Rectangle buttonBlankBounds{ Rectangle(307, 830, 144, 108) };

    LoopmanMomentaryButton loopButton, stopButton, undoButton, redoButton;
    //juce::ImageButton undoButton, redoButton;
    const Rectangle loopButtonBounds{ Rectangle(390, 933, 148, 110) / backgroundScale };
    const Rectangle stopButtonBounds{ Rectangle(563, 933, 148, 110) / backgroundScale };
    const Rectangle undoButtonBounds{ Rectangle( 81, 901,  98,  98) / backgroundScale };
    const Rectangle redoButtonBounds{ Rectangle(206, 901,  98,  98) / backgroundScale };

    juce::AudioProcessorValueTreeState::ButtonAttachment
        loopButtonAttachment, stopButtonAttachment, undoButtonAttachment, redoButtonAttachment;

    LoopPosition loopPosition;
    const Rectangle loopPositionBounds{ Rectangle( 62, 622, 257, 257) / backgroundScale };

    LoopmanMemoryBar memorySlider;
    const Rectangle memoryBarBounds{      Rectangle(601, 632, 134, 252) / backgroundScale };
    const Rectangle memoryBarTrackBounds{ Rectangle(655, 636,  28, 196) / backgroundScale };

    const int timerMilliseconds { 25 };
    const int timerRepaintCounts { 4 };
    int timerCount { 0 };

    void updateLoopControls();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopmanPIAudioProcessorEditor)
};
