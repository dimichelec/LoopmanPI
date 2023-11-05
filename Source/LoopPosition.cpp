/*
  ==============================================================================

    LoopPosition.cpp
    Created: 5 Nov 2023 2:32:53am
    Author:  dimic

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LoopPosition.h"

//==============================================================================
LoopPosition::LoopPosition()
{
}

LoopPosition::~LoopPosition()
{
}

void LoopPosition::paint (juce::Graphics& g)
{
    // update playState label
    juce::String txt{ "" };
    auto loops = looper->numLoops;
    if (loops > 0)
        txt = juce::String(loops) + " loop" + ((loops > 1) ? "s" : "");
    g.setColour (juce::Colours::orangered);
    g.setFont (22.0f);
    g.drawText (txt, getBounds(), juce::Justification::centred, true);


    // draw loop poisiton
    float x0 = (float)getX(), y0 = (float)getY();
    float cx = x0 + (float)getWidth() / 2;
    float cy = y0 + (float)getHeight() / 2;
    float rad = (float)getWidth() / 2 - 20;

    g.setColour(getLookAndFeel().findColour(juce::TextButton::buttonColourId));
    g.drawEllipse(cx - rad, cy - rad, rad * 2, rad * 2, 2);

    float pos = looper->getPlayPosition();
    if (pos < 0)
    {
        g.setColour(juce::Colours::goldenrod);
        g.drawEllipse(cx - rad, cy - rad, rad * 2, rad * 2, 8);
    }
    else {
        juce::PathStrokeType stroke(8);
        stroke.setEndStyle(juce::PathStrokeType::EndCapStyle::rounded);
        juce::Path path;
        path.addCentredArc(cx, cy, rad, rad, 0, 0, pos * 2 * juce::float_Pi, true);
        g.setColour((looper->isRecording()) ? juce::Colours::red : juce::Colours::green);
        g.strokePath(path, stroke);
    }
}

void LoopPosition::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..


}
