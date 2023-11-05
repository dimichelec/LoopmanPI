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
        mixAttachment       (p.state, "mix",        mixKnob),
        loopButtonAttachment(p.state, "loopButton", loopButton),
        stopButtonAttachment(p.state, "stopButton", stopButton),
        undoButtonAttachment(p.state, "undoButton", undoButton),
        redoButtonAttachment(p.state, "redoButton", redoButton)
{
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    setSize(backgroundImage.getWidth() / backgroundScale, backgroundImage.getHeight() / backgroundScale);

    // loop position
    loopPosition.setLooper(looper);

    // mix knob
    addAndMakeVisible(mixKnob);

    // loop button
    addAndMakeVisible(loopButton);
    juce::Image normalButton = juce::ImageCache::getFromMemory(BinaryData::loopButton_png, BinaryData::loopButton_pngSize);
    loopButton.setImages(true, false, true, normalButton, 1.0f, {}, {}, 1.0f, {}, {}, 0.5f, {});
    loopButton.onClick = [this]() { looper->loopClick(); };

    // stop button
    addAndMakeVisible(stopButton);
    normalButton = juce::ImageCache::getFromMemory(BinaryData::stopButton_png, BinaryData::stopButton_pngSize);
    stopButton.setImages(true, false, true, normalButton, 1.0f, {}, {}, 1.0f, {}, {}, 0.5f, {});
    stopButton.onClick = [this]() { looper->stopLoop(); };

    // undo button
    addAndMakeVisible(undoButton);
    normalButton = juce::ImageCache::getFromMemory(BinaryData::undoButton_png, BinaryData::undoButton_pngSize);
    undoButton.setImages(true, false, true, normalButton, 1.0f, {}, {}, 1.0f, {}, {}, 0.5f, {});
    undoButton.onClick = [this]() { looper->undoClick(); };

    // redo button
    addAndMakeVisible(redoButton);
    normalButton = juce::ImageCache::getFromMemory(BinaryData::redoButton_png, BinaryData::redoButton_pngSize);
    redoButton.setImages(true, false, true, normalButton, 1.0f, {}, {}, 1.0f, {}, {}, 0.5f, {});
    redoButton.onClick = [this]() { looper->redoClick(); };

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
    g.fillAll(juce::Colours::white);
    g.drawImageWithin(backgroundImage, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

    loopPosition.paint(g);

    // color memory usage bar
    auto color = getLookAndFeel().findColour(juce::Label::backgroundColourId);
    if (memoryUsage != 0.0) color = juce::Colours::green;
    else color = getLookAndFeel().findColour(juce::ProgressBar::backgroundColourId);
    memoryUsageBar.setColour(juce::ProgressBar::backgroundColourId, color);
}

void LoopmanPIAudioProcessorEditor::resized()
{
    const int controlsTop = 190;

    juce::Rectangle<int> bounds = getLocalBounds();
    const int margin = 20, smallMargin = 5, bigMargin = 30, topMargin = 15;
    const int mixKnobSize = 120, loopPositionSize = 150;
    
    // mix knob
    mixKnob.setBounds(getWidth() - mixKnobSize - bigMargin - 15, controlsTop + topMargin, mixKnobSize, mixKnobSize);

    // loop position
    juce::Rectangle<int> loopPositionBounds = juce::Rectangle<int>(bigMargin, controlsTop, loopPositionSize, loopPositionSize);
    loopPosition.setBounds(loopPositionBounds);

    // undo/redo buttons
    const int buttonSize = 50;
    juce::Rectangle<int> undoButtonBounds = juce::Rectangle<int>(
        loopPositionBounds.getCentreX() - buttonSize - smallMargin / 2, loopPositionBounds.getBottom() - smallMargin, buttonSize, buttonSize);
    undoButton.setBounds(undoButtonBounds);
    juce::Rectangle<int> redoButtonBounds = undoButtonBounds.translated(buttonSize + smallMargin, 0);
    redoButton.setBounds(redoButtonBounds);
        
    // loop/stop buttons
    const int buttonTop = getHeight() - 97 - smallMargin;
    juce::Rectangle<int> loopButtonBounds = juce::Rectangle<int>(smallMargin, buttonTop, getWidth() / 2, 100);
    loopButton.setBounds(loopButtonBounds);
    juce::Rectangle<int> stopButtonBounds = juce::Rectangle<int>(getWidth() - 190 - smallMargin, buttonTop, getWidth() / 2, 100);
    stopButton.setBounds(stopButtonBounds);
    
    // memory usage bar
    const int barHeight = 24;
    juce::Rectangle<int> memoryUsageBarBounds = juce::Rectangle<int>(margin, undoButton.getBottom() + margin, getWidth() - (2 * margin), barHeight);
    memoryUsageBar.setBounds(memoryUsageBarBounds);
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