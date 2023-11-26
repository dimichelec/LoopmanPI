/*
  ==============================================================================

    LoopmanMemoryBar.h
    Created: 12 Nov 2023 11:36:59am
    Author:  dimic

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using String = juce::String;
using Slider = juce::Slider;
using Rectangle = juce::Rectangle<int>;
using Colour = juce::Colour;

//==============================================================================
/*
*/
class LoopmanMemoryBar  : public Slider
{
public:
    LoopmanMemoryBar()
    {
        setSliderStyle(Slider::LinearBarVertical);
        setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        setRange(0.0, 1.0, 0.001);
        setEnabled(false);
        setColour(Slider::trackColourId, juce::Colours::green);
    }

    ~LoopmanMemoryBar() override {}

    void paint (juce::Graphics& g) override
    {
        // draw text
        String txt = (paintLabel) ? label : String(getValue() * 100, 1) + "%";
        g.setColour(labelColor);
        g.setFont(labelSize);
        const int fontHeight = (int)g.getCurrentFont().getHeight();
        const int fontOffset = -1;
        g.drawText(txt,
            Rectangle(0, getHeight() - fontHeight - fontOffset, getWidth(), fontHeight), juce::Justification::centred, false);

        // draw memory usage bar
        g.setColour(labelColor);
        g.fillRect(trackBounds);
        g.setColour(barColor);
        Rectangle bar{ trackBounds - getBounds().getPosition() };

        if (horizontal)
        {
            int newWidth = (int)((double)bar.getWidth() * getValue());
            if ((newWidth == 0) && (getValue() > 0)) newWidth = 1;
            //bar.translate(0, bar.getHeight() - newHeight + 1);
            bar.setWidth(newWidth);
        }
        else
        {
            int newHeight = (int)((double)bar.getHeight() * getValue());
            if ((newHeight == 0) && (getValue() > 0)) newHeight = 1;
            bar.translate(0, bar.getHeight() - newHeight + 1);
            bar.setHeight(newHeight);
        }
        g.fillRect(bar);
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

    void setLabel(String txt, float size, Colour color) { label = txt; labelSize = size; labelColor = color; }
    void setTrackBounds(Rectangle bounds)               { trackBounds = bounds; }
    void setHorizontal(bool horiz)                      { horizontal = horiz; }

private:
    bool    paintLabel  { true };
    String  label       { "" };
    float   labelSize   { 18.0f };
    Colour  labelColor  { juce::Colours::white };
    bool    horizontal  { true };

    Rectangle trackBounds;
    const Colour barColor = juce::Colours::green;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopmanMemoryBar)
};
