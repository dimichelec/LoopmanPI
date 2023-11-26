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

using Colour = juce::Colour;
using String = juce::String;

//==============================================================================
/*
*/
class LoopPosition  : public juce::Component
{
public:
    LoopPosition() {}
    ~LoopPosition() override {}

    void paint(juce::Graphics& g) override
    {
        // update playState label
        String txt{ "" };
        auto loops = looper->numLoops;
        if (loops > 0)
            txt = String(loops) + ((label == "") ? "" : " " + label + ((loops > 1) ? "s" : ""));
        g.setColour(labelColor);
        g.setFont(labelSize);
        g.drawText(txt, getBounds().translated(labelOffsetX, labelOffsetY), juce::Justification::centred, true);

        // draw loop poisiton
        float x0 = (float)getX(), y0 = (float)getY();
        float cx = x0 + (float)getWidth() / 2;
        float cy = y0 + (float)getHeight() / 2;
        float rad = (float)getWidth() / 2 - trackWeight / 2;

        float pos = looper->getPlayPosition();
        if (pos < 0)
        {
            g.setColour(juce::Colours::goldenrod);
            g.drawEllipse(cx - rad, cy - rad, rad * 2, rad * 2, trackWeight);
        }
        else
        {
            juce::PathStrokeType stroke(trackWeight);
            stroke.setEndStyle(juce::PathStrokeType::EndCapStyle::rounded);
            juce::Path path;
            path.addCentredArc(cx, cy, rad, rad, 0, 0, pos * 2 * juce::float_Pi, true);
            Colour c = juce::Colours::green;
            if (looper->isRecording()) c = juce::Colours::red;
            else if (looper->isFadingout()) c = juce::Colours::goldenrod;
            g.setColour(c);
            g.strokePath(path, stroke);
        }
    }

    void resized() override {}

    void setLooper(Looper* ptr)             { looper = ptr; }
    void setLabel(String txt, float size, Colour color) { label = txt; labelSize = size; labelColor = color; }

private:
    Looper* looper          {};
    String  label           { "" };
    float   labelSize       { 100.0f };
    Colour  labelColor      { juce::Colours::white };
    int     labelOffsetX    { 1 };
    int     labelOffsetY    { -4 };

    float trackWeight       { 11.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopPosition)
};
