#pragma once

#include <JuceHeader.h>


class Looper: public juce::Timer
{
public:
    Looper() {}
    ~Looper() {}

    void prepare(double rate)
    {
        sampleRate = rate;
        buffer.setSize(bufferChannels, (int)std::round(rate * bufferSeconds));
        bufferSize = buffer.getNumSamples();
        resetLoop();
    }

    void resetLoop()
    {
        buffer.clear();
        playHead = numLoops = recordedLoops = loopStart[0] = loopEnd[0] = 0;
        playState = recordState = false;
    }

    void loopClick()
    {
        if (!playState && !recordState)
        {
            if (numLoops == 0)
            {
                resetLoop();
                numLoops = 1;
                recordState = true;
            }
            else
            {
                playHead = 0;
                playState = true;
            }
        }
        else if (recordState)
        {
            loopEnd[numLoops - 1]--;
            if (numLoops == 1) playHead = 0;
            playState = true;
            recordState = false;
            recordedLoops = numLoops;
        }
        else // playState
        {
            if (numLoops >= maxLoops) return;
            if (bufferSize < (loopSize() * (numLoops + 1))) return;
            int nextPos = loopSize() * numLoops++;
            buffer.clear(nextPos, loopSize());
            loopStart[numLoops - 1] = nextPos;
            loopEnd[numLoops - 1] = nextPos + playHead;
            recordState = true;
        }
        repaintRequest = true;
    }

    void stopRecording()
    {
        if (!recordState) return;
        if (numLoops == 1)
        {
            loopEnd[0]--;
        }
        else
        {
            loopEnd[numLoops - 1] = loopStart[numLoops - 1] + loopEnd[0];
        }
        playHead = 0;
        playState = true;
        recordState = false;
        recordedLoops = numLoops;
    }

    void stopPlaying()
    {
        if (!playState) return;
        playState = false;
        stopFadeout();
        repaintRequest = true;
    }

    void stopFadeout()
    {
        if (!isFadingout()) return;
        fadeoutGain = 0.0f;
        stopPlaying();
        fadeoutState = false;
        stopTimer();
    }

    void stopClick()
    {
        if (!playState && !recordState)
        {
            resetLoop();
            repaintRequest = true;
        }
        else if (recordState)
        {
            stopRecording();
        }
        else // playState
        {
            stopPlaying();
        }
    }

    void fadeoutClick()
    {
        if (!isPlaying()) return;
        stopRecording();
        fadeoutStep = (float)(timerMilliseconds * 0.001f / loopTime());
        fadeoutState = true;
        startTimer(timerMilliseconds);
    }
    
    void undoClick()
    {
        if ((numLoops > 1) && !recordState) numLoops--;
    }

    void redoClick()
    {
        if ((numLoops < recordedLoops) && !recordState) numLoops++;
    }

    double getUsage() { return (numLoops < 1) ? 0 : (double)loopEnd[ numLoops-1 ] / (double)buffer.getNumSamples(); }

    float getPlayPosition() {
        if (!playState)
        {
            if (recordState) return -1;
            return 0;
        }
        return (float)playHead / (float)loopEnd[0];
    }

    bool getRepaintRequest() { return repaintRequest; }
    void setRepaintRequest(bool request) { repaintRequest = request; }

    bool isPlaying() { return playState; }
    bool isRecording() { return recordState; }
    bool isFadingout() { return fadeoutState; }

    float getFadeoutGain() { return fadeoutGain; }
    void setFadeoutGain(float gain) { fadeoutGain = gain; }

    void addLoopSample(int channel, float sample)
    {
        if (!recordState) return;
        if (loopEnd[numLoops - 1] >= buffer.getNumSamples()) return;
        buffer.addSample(channel, loopEnd[numLoops - 1]++, sample);
        if (loopEnd[numLoops - 1] >= bufferSize)
            loopEnd[numLoops - 1] = loopStart[numLoops - 1];
        if ((numLoops > 1) && (loopEnd[numLoops - 1] >= (loopStart[numLoops - 1] + loopSize())))
            loopEnd[numLoops - 1] -= loopSize();
    }

    float getPlaySample(int channel)
    {
        if (!playState || (numLoops < 1)) return 0;
        float sample = 0;
        for (auto loop = 1; loop <= numLoops; ++loop)
            sample += peekPlaySample(channel, loop);
        advancePlayHead();
        return sample;
    }

    static const int maxLoops{ 20 };

    int playHead{ 0 };
    int numLoops{ 0 };
    int recordedLoops{ 0 };
    int loopStart[maxLoops]{ };
    int loopEnd[maxLoops]{ };
    int loopSize() { return ((numLoops == 0) ? 0 : loopEnd[0] + 1); }
    int getNumChannels() { return buffer.getNumChannels(); }
    double loopTime() { return ((double)loopSize() / (double)getNumChannels() / sampleRate); }
    
    const int timerMilliseconds = 100;

private:
    juce::AudioBuffer<float> buffer;
    const float bufferMinutes{ 10.0f };
    const float bufferSeconds{ bufferMinutes * 60.0f };
    const int   bufferChannels{ 2 };
    int bufferSize{ 0 };
    double sampleRate{ 0.0 };

    bool playState{ false };
    bool recordState{ false };
    bool fadeoutState{ false };

    float fadeoutStep{ 0.0f };
    float fadeoutGain{ 1.0f };

    bool repaintRequest{ false };

    bool advancePlayHead()
    {
        if (++playHead > loopEnd[0]) playHead = 0;
        return (playHead == 0);
    }

    float peekPlaySample(int channel, int loop)
    {
        auto pos = loopStart[loop - 1] + playHead;
        return buffer.getSample(channel, pos);
    }

    void timerCallback()
    {
        if (!isFadingout())
        {
            stopTimer();
            return;
        }
        if (fadeoutGain > fadeoutStep) fadeoutGain -= fadeoutStep;
        else stopFadeout();
    }

};

