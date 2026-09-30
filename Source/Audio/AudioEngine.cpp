#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
}

ModulationSource& AudioEngine::getModulationSource()
{
    return modulationSource;
}

void AudioEngine::setGain(float newGain)
{
    gain.store(newGain);
}

void AudioEngine::setCutoff(float newCutoff)
{
    cutoff.store(newCutoff);
}

void AudioEngine::load(const juce::File& file)
{
    wavPlayer.load(file);
}

void AudioEngine::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    wavPlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);

    smoothedGain.reset(sampleRate, 0.02);
    smoothedGain.setCurrentAndTargetValue(gain.load());

    
    // lfoMain.prepareToPlay(sampleRate);
    svf.prepareToPlay(sampleRate);
    svf.reset();
}

void AudioEngine::processBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* buffer = bufferToFill.buffer;

    const int numSamples = bufferToFill.numSamples;
    const int startSample = bufferToFill.startSample;

    constexpr float modDepth = 6000.0f;

    const float baseCutoff = cutoff.load();

    wavPlayer.processBlock(bufferToFill);

    smoothedGain.setTargetValue(gain.load());

    // Read pointers for Left/Right channels
    const float* inLeft = buffer->getReadPointer(0, startSample);

    const float* inRight = (buffer->getNumChannels() > 1) ? buffer->getReadPointer(1, startSample) : inLeft;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float currentGain = smoothedGain.getNextValue();

        float modRaw = modulationSource.getNextSample();

        if (!std::isfinite(modRaw))
            modRaw = 0.5f;

        const float modBipolar =
            (modRaw * 2.0f) - 1.0f;

        const float modCutoffOffset =
            modBipolar * modDepth;
        
        const float targetCutoff =
            baseCutoff + modCutoffOffset;

        svf.setCutoff(targetCutoff);

        float leftValue =
            svf.processSample(inLeft[sample], 0);

        float rightValue =
            svf.processSample(inRight[sample], 1);

        leftValue *= currentGain;
        rightValue *= currentGain;

        if (buffer->getNumChannels() >= 2)
        {
            buffer->setSample(0, startSample + sample, leftValue);

            buffer->setSample(1, startSample + sample, rightValue);
        }
    }
}

void AudioEngine::releaseResources()
{
    wavPlayer.releaseResources();
}

void AudioEngine::play()
{
    wavPlayer.play();
}

void AudioEngine::stop()
{
    wavPlayer.stop();
}