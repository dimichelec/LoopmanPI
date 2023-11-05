/*
  ==============================================================================

    LoopmanKnob.h
    Created: 3 Nov 2023 1:48:55pm
    Author:  dimic

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LoopmanKnob  : public juce::Slider
{
public:
    LoopmanKnob();
    ~LoopmanKnob() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Image image;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopmanKnob)
};
