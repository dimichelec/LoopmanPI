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
        loopLevelAttachment (p.state, "loopLevel",  loopLevelKnob),
        loopButtonAttachment(p.state, "loopButton", loopButton),
        stopButtonAttachment(p.state, "stopButton", stopButton),
        undoButtonAttachment(p.state, "undoButton", undoButton),
        redoButtonAttachment(p.state, "redoButton", redoButton)
{
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    setSize(backgroundImage.getWidth() / backgroundScale, backgroundImage.getHeight() / backgroundScale);

    buttonBlankImage = juce::ImageCache::getFromMemory(BinaryData::buttonBlank_png, BinaryData::buttonBlank_pngSize).rescaled(
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
    loopButton.setImages(false, false, false, buttonBlankImage.getClippedImage(loopButtonBounds), 0.0f, {}, {}, 0.0f, {}, {}, 0.7f, {});
    loopButton.onClick = [this]() { looper->loopClick(); };

    // stop button
    addAndMakeVisible(stopButton);
    stopButton.setBounds(stopButtonBounds);
    stopButton.setImages(false, false, false, buttonBlankImage.getClippedImage(stopButtonBounds), 0.0f, {}, {}, 0.0f, {}, {}, 0.7f, {});
    stopButton.onClick = [this]() { looper->stopLoop(); };

    // undo button
    addAndMakeVisible(undoButton);
    undoButton.setBounds(undoButtonBounds);
    undoButton.setImages(false, false, false, buttonBlankImage.getClippedImage(undoButtonBounds), 0.0f, {}, {}, 0.0f, {}, {}, 0.7f, {});
    undoButton.onClick = [this]() { looper->undoClick(); };

    // redo button
    addAndMakeVisible(redoButton);
    redoButton.setBounds(redoButtonBounds);
    redoButton.setImages(false, false, false, buttonBlankImage.getClippedImage(redoButtonBounds), 0.0f, {}, {}, 0.0f, {}, {}, 0.7f, {});
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
void LoopmanPIAudioProcessorEditor::paint (juce::Graphics& g)
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