#pragma once

#include <JuceHeader.h>


class Looper
{
public:
    Looper();
    ~Looper();

    void prepare(double sampleRate);
    void resetLoop();
    void loopClick();
    void stopLoop();

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

    void setLoopSample(int channel, float sample);
    float getPlaySample(int channel);

    static const int maxLoops{ 10 };

    int playHead{ 0 };
    int numLoops{ 0 };
    int loopStart[maxLoops]{ };
    int loopEnd[maxLoops]{ };
    int loopSize() { return ((numLoops == 0) ? 0 : loopEnd[0] + 1); }

private:
    juce::AudioBuffer<float> buffer;
    const float bufferMinutes{ 10.0f };
    const float bufferSeconds{ bufferMinutes * 60.0f };
    const int   bufferChannels{ 2 };
    int bufferSize{ 0 };

    bool advancePlayHead();

    float peekPlaySample(int channel, int loop);

    bool repaintRequest{ false };

};

