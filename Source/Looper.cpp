#include "Looper.h"


Looper::Looper() {

}

Looper::~Looper() {

}

void Looper::prepare(double sampleRate)
{
    buffer.setSize(bufferChannels, (int)std::round(sampleRate * bufferSeconds));
    bufferSize = buffer.getNumSamples();
    resetLoop();
}

void Looper::resetLoop()
{
    buffer.clear();
    playHead = numLoops = loopStart[0] = loopEnd[0] = 0;
    playState = recordState = false;
}

void Looper::loopClick()
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
        }
        buffer.clear(nextPos, loopSize());
        loopStart[numLoops - 1] = nextPos;
        loopEnd[numLoops - 1] = nextPos + playHead;
        recordState = true;
    }
    repaintRequest = true;
}

void Looper::stopLoop()
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

void Looper::setLoopSample(int channel, float sample)
{
    if (!recordState) return;
    buffer.setSample(channel, loopEnd[numLoops - 1]++, sample);
    if (loopEnd[numLoops - 1] >= bufferSize)
        loopEnd[numLoops - 1] = loopStart[numLoops - 1];
    if ((numLoops > 1) && (loopEnd[numLoops - 1] >= (loopStart[numLoops - 1] + loopSize())))
        loopEnd[numLoops - 1] -= loopSize();
}

float Looper::peekPlaySample(int channel, int loop)
{
    auto pos = loopStart[loop - 1] + playHead;
    return buffer.getSample(channel, pos);
}

bool Looper::advancePlayHead()
{
    if (++playHead > loopEnd[0]) playHead = 0;
    return (playHead == 0);
}

float Looper::getPlaySample(int channel)
{
    if (!playState || (numLoops < 1)) return 0;
    float sample = 0;
    for (auto loop = 1; loop <= numLoops; ++loop)
        sample += peekPlaySample(channel, loop);
    advancePlayHead();
    return sample;
}

