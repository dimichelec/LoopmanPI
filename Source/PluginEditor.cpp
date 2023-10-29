/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
LoopmanPIAudioProcessorEditor::LoopmanPIAudioProcessorEditor (LoopmanPIAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
        gainAttachment          (p.state, "gain",       gainSlider),
        loopLevelAttachment     (p.state, "loopLevel",  loopLevelKnob),
        loopButtonAttachment    (p.state, "loopButton", loopButton),
        stopButtonAttachment    (p.state, "stopButton", stopButton)
{
    setSize (400, 300);

    // erase boxes around slider control value textboxes
    getLookAndFeel().setColour(juce::Slider::textBoxOutlineColourId, getLookAndFeel().findColour(juce::Slider::textBoxBackgroundColourId));

    // loop level control
    addAndMakeVisible(loopLevelLabel);
    loopLevelLabel.setText("Loop Level", juce::dontSendNotification);
    loopLevelLabel.setJustificationType(juce::Justification::centred);
    loopLevelLabel.attachToComponent(&loopLevelKnob, false);

    addAndMakeVisible(loopLevelKnob);
    loopLevelKnob.setSliderStyle(juce::Slider::Rotary);
    loopLevelKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, loopLevelKnob.getWidth(), 30);

    // looper position and status component
    addAndMakeVisible(loopPosition);
    addAndMakeVisible(playStateLabel);
    playStateLabel.setJustificationType(juce::Justification::centred);

    // output gain control
    addAndMakeVisible(gainLabel);
    gainLabel.setText("Output", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.attachToComponent(&gainSlider, false);

    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(juce::Slider::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 30);

    // loop button
    addAndMakeVisible(loopButton);
    loopButton.setButtonText("LOOP");
    loopButton.onClick = [this]() { looper->loopClick(); };

    // stop button
    addAndMakeVisible(stopButton);
    stopButton.setButtonText("STOP");
    stopButton.onClick = [this]() { looper->stopLoop(); };

    // memory usage bar
    addAndMakeVisible(memoryUsageBar);
    memoryUsageBar.setStyle(juce::ProgressBar::Style::linear);

    // set a timer for servicing the UI periodically
    startTimer(timerMilliseconds);
}

LoopmanPIAudioProcessorEditor::~LoopmanPIAudioProcessorEditor()
{
}


//==============================================================================
void LoopmanPIAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // the play state indicator
    auto loops = looper->numLoops;
    juce::String txt{ "" };
    if (loops > 0)
        txt = juce::String(loops) + " loop" + ((loops > 1) ? "s" : "");
    playStateLabel.setText(txt, juce::dontSendNotification);

    // draw loop poisiton
    float x0 = (float)loopPosition.getX(), y0 = (float)loopPosition.getY();
    float cx = x0 + (float)loopPosition.getWidth() / 2;
    float cy = y0 + (float)loopPosition.getHeight() / 2;
    float rad = (float)loopPosition.getWidth() / 2 - 20;

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

    // color memory usage bar
    auto color = getLookAndFeel().findColour(juce::Label::backgroundColourId);
    if (memoryUsage != 0.0) color = juce::Colours::green;
    else color = getLookAndFeel().findColour(juce::ProgressBar::backgroundColourId);
    memoryUsageBar.setColour(juce::ProgressBar::backgroundColourId, color);
}

void LoopmanPIAudioProcessorEditor::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    const int margin = 20, smallMargin = 5, topMargin = 15;
    
    // output gain control
    juce::Rectangle<int> gainBounds = bounds.removeFromRight(getWidth() / 4);
    gainSlider.setBounds(gainBounds.reduced(margin).translated(0, topMargin));

    // knobs
    juce::Rectangle<int> knobsBounds = bounds.removeFromTop(getHeight() / 2).translated(0, topMargin);
    juce::Rectangle<int> loopLevel = knobsBounds.removeFromLeft(knobsBounds.getWidth() / 2);
    loopLevelKnob.setBounds(loopLevel.reduced(margin));
    loopPosition.setBounds(knobsBounds.reduced(smallMargin, margin/2).translated(0, -margin/2));

    // play state label
    playStateLabel.setBounds(
        loopPosition.getBounds().removeFromTop(
            loopPosition.getHeight() * 2 / 3
        ).removeFromBottom(30).reduced(30,0).translated(0, -7)
    );

    // buttons
    juce::Rectangle<int> buttonsBounds = bounds.removeFromTop(getHeight() / 4);
    juce::Rectangle<int> loopButtonBounds = buttonsBounds.removeFromLeft(buttonsBounds.getWidth() / 2);
    loopButton.setBounds(loopButtonBounds.reduced(smallMargin));
    stopButton.setBounds(buttonsBounds.reduced(smallMargin));

    // memory usage bar
    const int barHeight = 24;
    juce::Rectangle<int> memoryUsageBarBounds = bounds.removeFromTop(barHeight + smallMargin * 2).removeFromLeft(bounds.getWidth() / 2);
    memoryUsageBar.setBounds(memoryUsageBarBounds.reduced(smallMargin));
}

void LoopmanPIAudioProcessorEditor::buttonClicked(juce::Button*) // button)
{

}

void LoopmanPIAudioProcessorEditor::timerCallback()
{
    if ((timerCount++ >= timerRepaintCounts) || looper->getRepaintRequest())
    {
        timerCount = 0;
        looper->setRepaintRequest(false);
        updateLoopControls();
    }
}

void LoopmanPIAudioProcessorEditor::updateLoopControls()
{
    memoryUsage = looper->getUsage();
    repaint();
}