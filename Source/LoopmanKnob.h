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
    LoopmanKnob()
    {
        setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        setColour(juce::Slider::ColourIds::thumbColourId, dotColor);
        setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colour(0x00000000));
        setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colour(0x00000000));
        setDoubleClickReturnValue(true, 1.0f);
    }

    ~LoopmanKnob() override {}

    void paint(juce::Graphics& g) override {
        // draw text under knob
        juce::String txt = (paintLabel) ? label : juce::String(getValue() * 100, 0) + "%";
        g.setColour(labelColor);
        g.setFont(20.0f);
        const int fontHeight = (int)g.getCurrentFont().getHeight();
        const int fontOffset = 5;
        g.drawText(txt,
            juce::Rectangle<int>(0, getHeight() - fontHeight - fontOffset, getWidth(), fontHeight), juce::Justification::centred, false);

        // draw the knob dot
        float dotDistance = 0.40f;
        float dotSize = 6;
        float r = getWidth() / 2.0f, cx = r - 3, cy = r - 3;
        float ang = (-2.0f / 8.0f * juce::float_Pi) - ((float)getValue() * 3.0f / 2.0f * juce::float_Pi);
        float sx = cx + (r * dotDistance * sin(ang)), sy = cy + (r * dotDistance * cos(ang));
        g.setColour(juce::Colours::black);
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

    void setLabel(juce::String txt) { label = txt; }

private:
    bool paintLabel{ true };
    juce::String label;
    const juce::Colour labelColor = juce::Colour(0xff2d1111);
    const juce::Colour dotColor = juce::Colours::black;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopmanKnob)
};
