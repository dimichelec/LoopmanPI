/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using ImageCache = juce::ImageCache;

//==============================================================================
LoopmanPIAudioProcessorEditor::LoopmanPIAudioProcessorEditor(LoopmanPIAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
        loopLevelAttachment     (p.state, "loopLevel",      loopLevelKnob),
        loopButtonAttachment    (p.state, "loopButton",     loopButton),
        stopButtonAttachment    (p.state, "stopButton",     stopButton),
        fadeoutButtonAttachment (p.state, "fadeoutButton",  fadeoutButton),
        undoButtonAttachment    (p.state, "undoButton",     undoButton),
        redoButtonAttachment    (p.state, "redoButton",     redoButton)
{
    backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    setSize(backgroundImage.getWidth() / backgroundScale, backgroundImage.getHeight() / backgroundScale);

    buttonBlankImage = ImageCache::getFromMemory(BinaryData::buttonBlank_png, BinaryData::buttonBlank_pngSize).rescaled(
        getWidth(), getHeight()
    );

    // loop position
    loopPosition.setLooper(looper);
    loopPosition.setBounds(loopPositionBounds);

    // loop level knob
    addAndMakeVisible(loopLevelKnob);
    loopLevelKnob.setBounds(loopLevelKnobBounds);
    loopLevelKnob.setLabel("Level");

    // loop button
    addAndMakeVisible(loopButton);
    loopButton.setBounds(loopButtonBounds);
    loopButton.setBlankImage(buttonBlankImage.getClippedImage(loopButtonBounds));
    loopButton.onClick = [this]() { looper->setFadeoutGain(1.0f); looper->loopClick(); };

    // stop button
    addAndMakeVisible(stopButton);
    stopButton.setBounds(stopButtonBounds);
    stopButton.setBlankImage(buttonBlankImage.getClippedImage(stopButtonBounds));
    stopButton.onClick = [this]() { looper->stopClick(); };

    // fadeout button
    addAndMakeVisible(fadeoutButton);
    fadeoutButton.setBounds(fadeoutButtonBounds);
    fadeoutButton.setBlankImage(buttonBlankImage.getClippedImage(fadeoutButtonBounds));
    fadeoutButton.onClick = [this]() { looper->fadeoutClick(); };

    // undo button
    addAndMakeVisible(undoButton);
    undoButton.setBounds(undoButtonBounds);
    undoButton.setBlankImage(buttonBlankImage.getClippedImage(undoButtonBounds));
    undoButton.onClick = [this]() { looper->undoClick(); };

    // redo button
    addAndMakeVisible(redoButton);
    redoButton.setBounds(redoButtonBounds);
    redoButton.setBlankImage(buttonBlankImage.getClippedImage(redoButtonBounds));
    redoButton.onClick = [this]() { looper->redoClick(); };

    // memory usage bar
    addAndMakeVisible(memorySlider);
    memorySlider.setBounds(memoryBarBounds);
    memorySlider.setTrackBounds(memoryBarTrackBounds);
    memorySlider.setLabel("Memory");

    // set a timer for servicing the UI periodically
    startTimer(timerMilliseconds);
}

LoopmanPIAudioProcessorEditor::~LoopmanPIAudioProcessorEditor()
{
}


//==============================================================================
void LoopmanPIAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
    g.drawImageWithin(backgroundImage, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

    loopPosition.paint(g);
}

void LoopmanPIAudioProcessorEditor::resized()
{
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
    memorySlider.setValue(looper->getUsage());
    repaint();
}