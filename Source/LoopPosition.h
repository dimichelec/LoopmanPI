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
using Point = juce::Point<float>;

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
        auto loops = looper->numLoops;
        if (loops > 0)
        {
            String txt{ String(loops) };
            if (label != "") txt += " " + label + ((loops > 1) ? "s" : "");
            g.setColour(labelColor);
            g.setFont(labelSize);
            g.drawText(txt, getBounds().translated(labelOffsetX, labelOffsetY), juce::Justification::centred, true);
        }

        // draw loop poisiton
        float pos = looper->getPlayPosition();
        if (pos < 0)
        {
            g.setColour(recordColor);
            g.drawEllipse(trackRect, trackWeight);
        }
        else
        {
            juce::PathStrokeType stroke(trackWeight);
            stroke.setEndStyle(juce::PathStrokeType::EndCapStyle::rounded);
            juce::Path path;
            path.addCentredArc(trackCenter.x, trackCenter.y, trackRadius, trackRadius, 0, 0, pos * 2 * juce::float_Pi, true);
            Colour c;
            if (looper->isRecording()) c = recordColor;
            else if (looper->isFadingout()) c = playColor.withAlpha(juce::jmin(0.2f + looper->getFadeoutGain(), 1.0f));
            else if (looper->isPlaying()) c = playColor;
            else if (!looper->isPlaying() && (looper->getFadeoutGain() == 0.0f))
            {
                g.setColour(fadedoutColor);
                g.drawEllipse(trackRect, trackWeight);
                return;
            }
            g.setColour(c);
            g.strokePath(path, stroke);
        }
    }

    void resized() override
    {
        trackCenter.setXY((float)getX() + (float)getWidth() / 2, (float)getY() + (float)getHeight() / 2);
        trackRadius = (float)getWidth() / 2 - trackWeight / 2;
        trackRect.setBounds(trackCenter.x - trackRadius, trackCenter.y - trackRadius, trackRadius * 2, trackRadius * 2);
    }

    void setLooper(Looper* ptr) { looper = ptr; }
    void setLabel(String txt, float size, Colour color) { label = txt; labelSize = size; labelColor = color; }

private:
    Looper* looper{};
    String  label{ "" };
    float   labelSize{ 100.0f };
    Colour  labelColor{ juce::Colours::white };
    int     labelOffsetX{ 1 };
    int     labelOffsetY{ -4 };

    float trackWeight{ 11.0f };
    const Colour playColor{ juce::Colours::green };
    const Colour recordColor{ juce::Colours::red };
    const Colour fadedoutColor{ juce::Colours::darkorange };

    Point trackCenter;
    float trackRadius{ 0.0f };
    juce::Rectangle<float> trackRect;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopPosition)
};
