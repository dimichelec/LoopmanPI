/*
  ==============================================================================

    LoopmanKnob.h
    Created: 3 Nov 2023 1:48:55pm
    Author:  dimic

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using String = juce::String;
using Slider = juce::Slider;
using Colour = juce::Colour;
using Rectangle = juce::Rectangle<int>;

//==============================================================================
/*
*/
class LoopmanKnob : public Slider
{
public:
    LoopmanKnob()
    {
        setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        setDoubleClickReturnValue(true, 1.0f);
    }

    ~LoopmanKnob() override {}

    void paint(juce::Graphics& g) override {
        // draw text under knob
        String txt = (paintLabel) ? label : String(getValue() * 100, 0) + "%";
        g.setColour(labelColor);
        g.setFont(labelSize);
        const int fontHeight = (int)g.getCurrentFont().getHeight();
        const int fontOffset = 5;
        g.drawText(txt,
            Rectangle(0, getHeight() - fontHeight - fontOffset, getWidth(), fontHeight),
            juce::Justification::centred, false);

        // draw the knob dot
        float r = getWidth() / 2.0f, cx = r - 3, cy = r - 3;
        float ang = (-2.0f / 8.0f * juce::float_Pi) - ((float)getValue() * 3.0f / 2.0f * juce::float_Pi);
        float sx = cx + (r * dotDistance * sin(ang)), sy = cy + (r * dotDistance * cos(ang));
        g.setColour(dotColor);
        g.fillEllipse(sx, sy, dotSize, dotSize);
    }

    void resized() override {}

    void mouseEnter(const juce::MouseEvent&) override
    {
        paintLabel = false;
    }

    void mouseExit(const juce::MouseEvent&) override
    {
        paintLabel = true;
    }

    void setLabel(String txt, float size, Colour color)   { label = txt; labelSize = size; labelColor = color; }
    void setDot(Colour color, float distance, float size) { dotColor = color; dotDistance = distance; dotSize = size; }

private:
    bool    paintLabel  { true };
    String  label       { "" };
    float   labelSize   { 18.0f };
    Colour  labelColor  { juce::Colours::white };
    Colour  dotColor    { juce::Colours::white };
    float   dotDistance { 0.5f };
    float   dotSize     { 6.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopmanKnob)
};
