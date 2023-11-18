/*
  ==============================================================================

    LoopmanMomentaryButton.h
    Created: 17 Nov 2023 8:56:03am
    Author:  dimic

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using Image = juce::Image;

//==============================================================================
/*
*/
class LoopmanMomentaryButton  : public juce::ImageButton
{
public:
    LoopmanMomentaryButton()
    {
        setTriggeredOnMouseDown(true);
    }

    ~LoopmanMomentaryButton() override
    {
    }

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        if (!isEnabled())
        {
            shouldDrawButtonAsHighlighted = false;
            shouldDrawButtonAsDown = false;
        }

        if (blankImage.isValid())
            getLookAndFeel().drawImageButton(g, &blankImage, 0, 0, blankImage.getWidth(), blankImage.getHeight(), 
                {}, (shouldDrawButtonAsDown ? buttonDownOpacity : 0.0f), *this);
    }

    void setBlankImage(Image image) { blankImage = image; }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    Image blankImage;
    float buttonDownOpacity{ 0.7f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopmanMomentaryButton)
};
