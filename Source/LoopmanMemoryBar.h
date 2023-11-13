/*
  ==============================================================================

    LoopmanMemoryBar.h
    Created: 12 Nov 2023 11:36:59am
    Author:  dimic

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LoopmanMemoryBar  : public juce::Slider
{
public:
    LoopmanMemoryBar()
    {
        setSliderStyle(juce::Slider::LinearBarVertical);
        setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        setRange(0.0, 1.0, 0.001);
        setEnabled(false);
        setColour(juce::Slider::trackColourId, juce::Colours::green);
    }

    ~LoopmanMemoryBar() override {}

    void paint (juce::Graphics& g) override
    {
        // draw text
        juce::String txt = (paintLabel) ? label : juce::String(getValue() * 100, 1) + "%";
        g.setColour(labelColor);
        g.setFont(20.0f);
        const int fontHeight = (int)g.getCurrentFont().getHeight();
        const int fontOffset = 2;
        g.drawText(txt,
            juce::Rectangle<int>(0, getHeight() - fontHeight - fontOffset, getWidth(), fontHeight), juce::Justification::centred, false);

        // draw memory usage bar
        g.setColour(labelColor);
        g.fillRect(trackBounds);
        g.setColour(barColor);
        juce::Rectangle<int> bar{ trackBounds - getBounds().getPosition() };
        int newHeight = (int)((double)bar.getHeight() * getValue());
        if ((newHeight == 0) && (getValue() > 0)) newHeight = 1;
        bar.translate(0, bar.getHeight() - newHeight + 1);
        bar.setHeight(newHeight);
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

    void setLabel(juce::String txt) { label = txt; }
    void setTrackBounds(juce::Rectangle<int> bounds) { trackBounds = bounds; }

private:
    bool paintLabel{ true };
    juce::String label;
    const juce::Colour labelColor = juce::Colour(0xff2d1111);

    juce::Rectangle<int> trackBounds;
    const juce::Colour barColor = juce::Colours::green;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopmanMemoryBar)
};
