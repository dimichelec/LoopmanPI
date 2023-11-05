/*
  ==============================================================================

    LoopmanKnob.cpp
    Created: 3 Nov 2023 1:48:55pm
    Author:  dimic

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LoopmanKnob.h"

//==============================================================================
LoopmanKnob::LoopmanKnob()
{
    setSliderStyle(juce::Slider::RotaryVerticalDrag);
    image = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
}

LoopmanKnob::~LoopmanKnob()
{
}

void LoopmanKnob::paint (juce::Graphics& g)
{
    g.drawImageWithin(image, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
    g.setColour (juce::Colours::orangered);
    g.setFont (24.0f);
    g.drawMultiLineText("Mix\n" + juce::String(getValue() * 100, 0) + "%",
        0, getHeight()/2 - 5, getWidth(), juce::Justification::centred, -3);

    float r = getWidth() / 2.0f, cx = r -3, cy = r - 3;
    float ang = (-2.0f / 8.0f * juce::float_Pi) - ((float)getValue() * 3.0f / 2.0f * juce::float_Pi);
    float sx = cx + (r * 0.65f * sin(ang)), sy = cy + (r * 0.65f * cos(ang));
    g.setColour(juce::Colours::black);
    g.fillEllipse(sx, sy, 7, 7);

}

void LoopmanKnob::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

