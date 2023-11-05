/*
  ==============================================================================

    LoopPosition.h
    Created: 5 Nov 2023 2:32:53am
    Author:  dimic

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Looper.h"

//==============================================================================
/*
*/
class LoopPosition  : public juce::Component
{
public:
    LoopPosition();
    ~LoopPosition() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setLooper(Looper* ptr) { looper = ptr; }

private:
    Looper* looper{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopPosition)
};
