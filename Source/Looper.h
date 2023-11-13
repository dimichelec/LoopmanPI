#pragma once

#include <JuceHeader.h>


class Looper
{
public:
    Looper() {}
    ~Looper() {}

    void prepare(double sampleRate)
    {
        buffer.setSize(bufferChannels, (int)std::round(sampleRate * bufferSeconds));
        bufferSize = buffer.getNumSamples();
        resetLoop();
    }

    void resetLoop()
    {
        buffer.clear();
        playHead = numLoops = maxRedoLoops = loopStart[0] = loopEnd[0] = 0;
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
        }
        else // playState
        {
            int nextPos = loopSize() * numLoops;
            if ((++numLoops > maxLoops) || (nextPos >= bufferSize))
            {
                nextPos = 0;
                numLoops = 1;
                maxRedoLoops = 0;
            }
            maxRedoLoops++;
            buffer.clear(nextPos, loopSize());
            loopStart[numLoops - 1] = nextPos;
            loopEnd[numLoops - 1] = nextPos + playHead;
            recordState = true;
        }
        repaintRequest = true;
    }

    void stopLoop()
    {
        if (!playState && !recordState)
        {
            resetLoop();
        }
        else if (recordState)
        {
            if (numLoops == 1)
            {
                loopEnd[0]--;
            }
            else
            {
                loopEnd[numLoops - 1] = loopStart[numLoops - 1] + loopEnd[0];
                maxRedoLoops++;
            }
            playHead = 0;
            recordState = false;
            playState = true;
        }
        else // playState
        {
            playState = false;
        }
        repaintRequest = true;
    }

    void undoClick()
    {
        if ((numLoops > 1) && !recordState) numLoops--;
    }

    void redoClick()
    {
        if ((maxRedoLoops > 0) && !recordState)
        {
            numLoops++;
            maxRedoLoops--;
        }
    }

    double getUsage() {
        return (double)loopEnd[ numLoops-1 ] / (double)buffer.getNumSamples();
    }

    float getPlayPosition() {
        if (!playState)
        {
            if (recordState) return -1;
            return 0;
        }
        return (float)playHead / (float)loopEnd[0];
    }

    bool getRepaintRequest() {
        return repaintRequest;
    }

    void setRepaintRequest(bool request) {
        repaintRequest = request;
    }

    bool playState{ false };
    bool recordState{ false };

    bool isPlaying() { return playState; }
    bool isRecording() { return recordState; }

    void addLoopSample(int channel, float sample)
    {
        if (!recordState) return;
        buffer.addSample(channel, loopEnd[numLoops - 1]++, sample);
        //buffer.setSample(channel, loopEnd[numLoops - 1]++, sample);
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

    static const int maxLoops{ 10 };

    int playHead{ 0 };
    int numLoops{ 0 };
    int maxRedoLoops{ 0 };
    int loopStart[maxLoops]{ };
    int loopEnd[maxLoops]{ };
    int loopSize() { return ((numLoops == 0) ? 0 : loopEnd[0] + 1); }

private:
    juce::AudioBuffer<float> buffer;
    const float bufferMinutes{ 10.0f };
    const float bufferSeconds{ bufferMinutes * 60.0f };
    const int   bufferChannels{ 2 };
    int bufferSize{ 0 };

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

    bool repaintRequest{ false };

};

